#pragma once
#include "..\..\source\stdafx.h"
#include "..\requests\read.hpp"
#include "..\requests\cr3.hpp"
#include "..\requests\low.h"
#include "..\..\utils\utilites.hpp"

extern "C" PVOID NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process);
extern "C" NTSTATUS NTAPI ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength);

namespace requests
{
	NTSTATUS handle_read(prw request)
	{
		if (!request->process_id) {
			return STATUS_UNSUCCESSFUL;
		}
		PEPROCESS PROCCSS = NULL;
		qtx_import(PsLookupProcessByProcessId)((HANDLE)(request->process_id), &PROCCSS);
		if (!PROCCSS) {
			return STATUS_UNSUCCESSFUL;
		}
		INT64 physicaladdress;
		physicaladdress = utils::translate_linear(m_stored_dtb, (ULONG64)(request->address));
		if (!physicaladdress) {
			return STATUS_UNSUCCESSFUL;
		}
		ULONG64 finalsize = find_min(PAGE_SIZE - (physicaladdress & 0xFFF), request->size);
		SIZE_T bytestrough = NULL;
		if (request->write) {
			read_physical(PVOID(physicaladdress), (PVOID)((ULONG64)(request->buffer)), finalsize, &bytestrough);
		}
		else
		{
			read_physical(PVOID(physicaladdress), (PVOID)((ULONG64)(request->buffer)), finalsize, &bytestrough);
		}
		return STATUS_SUCCESS;
	}
	NTSTATUS find_image(pba request) {
		if (!request->process_id) {
			return STATUS_UNSUCCESSFUL;
		}
		PEPROCESS processs = NULL;
		qtx_import(PsLookupProcessByProcessId)((HANDLE)request->process_id, &processs);
		if (!processs) {
			return STATUS_UNSUCCESSFUL;
		}
		ULONGLONG baseimg = (ULONGLONG)PsGetProcessSectionBaseAddress(processs);
		if (!baseimg) {
			return STATUS_UNSUCCESSFUL;
		}
		RtlCopyMemory(request->address, &baseimg, sizeof(baseimg));
		ObDereferenceObject(processs);
		return STATUS_SUCCESS;
	}
	NTSTATUS get_cr3(dtbl request)
	{
		dtb dtbdata = { 0 };
		if (!request->process_id) {
			return STATUS_UNSUCCESSFUL;
		}
		PEPROCESS procc = 0; 
		qtx_import(PsLookupProcessByProcessId)((HANDLE)request->process_id, &procc);
		if (!procc) {
			return STATUS_UNSUCCESSFUL;
		}
		m_stored_base = (void*)PsGetProcessSectionBaseAddress(procc);

		m_stored_dtb = pml4::dirbase_from_base_address(m_stored_base);

		ObDereferenceObject(procc);
		ULONGLONG raaa = 1;
		qtx_import(memcpy)(request->operation, &raaa, sizeof(raaa));
		return STATUS_SUCCESS;
	}
}
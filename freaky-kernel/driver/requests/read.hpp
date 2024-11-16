#pragma once
#include "..\..\source\stdafx.h"

auto read_physical(PVOID target_address,
	PVOID buffer,
	SIZE_T size,
	SIZE_T* bytes_read) -> NTSTATUS
{
	MM_COPY_ADDRESS to_read = { 0 };
	to_read.PhysicalAddress.QuadPart = (LONGLONG)target_address;
	return qtx_import(MmCopyMemory)(buffer, to_read, size, MM_COPY_MEMORY_PHYSICAL, bytes_read);
}

ULONG64 find_min(INT32 g,
	SIZE_T f)
{
	INT32 h = (INT32)f;
	ULONG64 result = 0;

	result = (((g) < (h)) ? (g) : (h));

	return result;
}
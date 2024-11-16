#pragma once
#include "communication\communication.hpp"

ULONGLONG m_stored_dtb;
PVOID m_stored_base;
PEPROCESS save_process;
uint64_t eac_module;
uint64_t eac_cr3;
#define PAGE_OFFSET_SIZE 12
static const UINT64 PMASK = (~0xfull << 8) & 0xfffffffffull;


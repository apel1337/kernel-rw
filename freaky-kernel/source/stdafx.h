#pragma once
///default nt lib
#include <ntifs.h>
#include <windef.h>
#include <strsafe.h>
#include <ntifs.h>
#include <intrin.h>
#include <ntddk.h>
#include <cstdint>
#include <ntdef.h>
///kernel modules
#include "..\kernel\kli.hpp"
#include "..\kernel\xorstr.hpp"
///communication modules
#include "..\driver\communication\communication.hpp"
#include "..\driver\communication\structs.h"
///driver definitions
#include "..\driver\driver_defs.h"
///request modules
#include "..\driver\requests\cr3.hpp"
#include "..\driver\handling\io_dispatch.hpp"
///encryption modules
#include "..\kernel\xorstr.hpp"
///stackwalking modules
#include "..\driver\nmi_stackwalk\stackwalk.hpp"
///util modules
#include "..\utils\utilites.hpp"
///crt modules
#include "..\utils\crt.h"
///modules
#include "..\utils\modules.hpp"
///oxorany modules
#include "..\utils\oxorany\oxorany_include.h"
///entry modules
#include "..\utils\entry\entry.hpp"




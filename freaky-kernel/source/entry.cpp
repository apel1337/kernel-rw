#include "stdafx.h"
/*
 ___             _          _                   _
|  _|___ ___ ___| |_ _ _   | |_ ___ ___ ___ ___| |
|  _|  _| -_| .'| '_| | |  | '_| -_|  _|   | -_| |
|_| |_| |___|__,|_,_|_  |  |_,_|___|_| |_|_|___|_|
					|___|

- made by rack

credits ->
- hopesar
- dvl
- interpreter
- redshirtfan
*/

NTSTATUS DriverEntry()
{
	if (!NT_SUCCESS(pml4::InitializeMmPfnDatabase()))
		return 0;

	return entry::main();
}


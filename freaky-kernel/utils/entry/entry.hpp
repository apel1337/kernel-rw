#pragma once

#include "..\..\source\stdafx.h"

extern "C" NTSTATUS NTAPI IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);

namespace entry
{
	NTSTATUS real_entry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
		UNREFERENCED_PARAMETER(registry_path);

		NTSTATUS status = { };
		PDEVICE_OBJECT device_obj = { };
		qtx_import(RtlInitUnicodeString)(&name, L"\\Device\\{4d36e97d-e325-11ce-bfc1-08002be10318}");
		qtx_import(RtlInitUnicodeString)(&link, L"\\DosDevices\\{4d36e97d-e325-11ce-bfc1-08002be10318}");

		status = qtx_import(IoCreateDevice)(driver_object, 0, &name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_obj);
		if (!NT_SUCCESS(status))
			return status;

		utils::hide_driver_object(driver_object);

		status = qtx_import(IoCreateSymbolicLink)(&link, &name);
		if (!NT_SUCCESS(status))
			return status;

		for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
			driver_object->MajorFunction[i] = &unsupported_dispatch;

		device_obj->Flags |= DO_BUFFERED_IO;
		driver_object->MajorFunction[IRP_MJ_CREATE] = &dispatch_handler;
		driver_object->MajorFunction[IRP_MJ_CLOSE] = &dispatch_handler;
		driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &io_controller;
		device_obj->Flags &= ~DO_DEVICE_INITIALIZING;

		driver_object->DriverSection = NULL; //etw lag avoiding

		return status;
	}

	NTSTATUS main()
	{
		NTSTATUS status = qtx_import(IoCreateDriver)(0, entry::real_entry);

		return status;
	}
}

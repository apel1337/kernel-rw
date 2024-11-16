#pragma once
#include "..\requests\read.hpp"
#include "handle_requests.hpp"
#include "..\requests\cr3.hpp"

NTSTATUS io_controller(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);
	NTSTATUS status = { };
	ULONG bytes = { };
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;
	ULONG size = stack->Parameters.DeviceIoControl.InputBufferLength;
	if (code == rw_code) {
		if (size == sizeof(_rw)) {
			prw req = (prw)(irp->AssociatedIrp.SystemBuffer);

			status = requests::handle_read(req);
			bytes = sizeof(_rw);
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if (code == dtbfix_code)
	{
		if (size == sizeof(_dtb)) {
			dtbl req = (dtbl)(irp->AssociatedIrp.SystemBuffer);

			status = requests::get_cr3(req);
			bytes = sizeof(_dtb);
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if (code == base_code) {
		if (size == sizeof(_ba)) {
			pba req = (pba)(irp->AssociatedIrp.SystemBuffer);
			status = requests::find_image(req);
			bytes = sizeof(_ba);
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	irp->IoStatus.Status = status;
	irp->IoStatus.Information = bytes;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return status;
}
NTSTATUS unsupported_dispatch(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS dispatch_handler(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);
	switch (stack->MajorFunction) {
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE:
		break;
	default:
		break;
	}
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#include "net_wfp_rdr.h"
#include "debug.h"


extern "C"
{
	_IRQL_requires_(PASSIVE_LEVEL)
	_IRQL_requires_same_
	_Function_class_(DRIVER_INITIALIZE)
	DRIVER_INITIALIZE DriverEntry;

};

DRIVER_UNLOAD unload;

PDEVICE_OBJECT g_device_object;


VOID unload(_In_ PDRIVER_OBJECT /*driverObject*/)
{
    DbgPrint("Driver unloaded\n");

    DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver unloaded\n");
}


void net_wfp_start_filter()
{
    FWPM_SERVICE_STATE  engine_status;


    engine_status = FwpmBfeStateGet();
}


_Success_(return == STATUS_SUCCESS)
NTSTATUS NwrFwpmBfeStateSubscribeChanges()
{
	NTSTATUS status = STATUS_SUCCESS;


	DBGPRINT(NWRFLTR_INFO_LEVEL, "start: %d\n", 4);


	return status;
}

_Success_(return == STATUS_SUCCESS)
NTSTATUS
net_device_dispatch(
    _In_    PDEVICE_OBJECT DeviceObject,
    _In_    PIRP PtrIrp
)
{
    NTSTATUS    status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(DeviceObject);
    UNREFERENCED_PARAMETER(PtrIrp);

    return status;
}


_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
_Function_class_(DRIVER_INITIALIZE)
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject,_In_ PUNICODE_STRING pRegistryPath)
{
    DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver loaded\n");

    WPP_INIT_TRACING(pDriverObject, pRegistryPath);

	NT_ASSERT(pDriverObject);
	NT_ASSERT(pRegistryPath);


    NTSTATUS    status = STATUS_SUCCESS;


	for (UINT i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
        pDriverObject->MajorFunction[i] = net_device_dispatch;
    pDriverObject->DriverUnload = unload;


	status = IoCreateDevice(pDriverObject, 0, NULL, FILE_DEVICE_UNKNOWN,
		    FILE_DEVICE_SECURE_OPEN, FALSE, &g_device_object);

	if (!NT_SUCCESS(status)) {
		return status;
	}


    if (status != STATUS_SUCCESS) {
        WPP_CLEANUP(pDriverObject);
    }

    DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver end\n");

    return status;
}


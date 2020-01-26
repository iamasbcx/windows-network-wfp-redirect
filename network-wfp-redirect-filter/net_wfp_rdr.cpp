

#include "net_wfp_rdr.h"



extern "C"
{
	_IRQL_requires_(PASSIVE_LEVEL)
	_IRQL_requires_same_
	_Function_class_(DRIVER_INITIALIZE)
	DRIVER_INITIALIZE DriverEntry;
};


DRIVER_UNLOAD unload;


VOID unload(_In_ PDRIVER_OBJECT /*driverObject*/)
{
    DbgPrint("Driver unloaded\n");
}


void net_wfp_start_filter()
{
    FWPM_SERVICE_STATE  engine_status;


    engine_status = FwpmBfeStateGet();
}

_IRQL_requires_same_
_Function_class_(DRIVER_INITIALIZE)
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject,_In_ PUNICODE_STRING pRegistryPath)
{
    DbgPrint("Driver loaded\n");

    WPP_INIT_TRACING(pDriverObject, pRegistryPath);

	NT_ASSERT(pDriverObject);
	NT_ASSERT(pRegistryPath);


    NTSTATUS    status = STATUS_SUCCESS;

    
    pDriverObject->DriverUnload = unload;


	if (status != STATUS_SUCCESS)
		WPP_CLEANUP(pDriverObject);

    return status;
}
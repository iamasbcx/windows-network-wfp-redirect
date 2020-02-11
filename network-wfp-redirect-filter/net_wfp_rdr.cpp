// Copyright (c) 2020 Alexandr Shevchenko. (alexshev@live.com) 
// All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "net_wfp_rdr.h"
#include "net_wfp_callout.h"
#include "debug.h"


extern "C"
{
	_IRQL_requires_(PASSIVE_LEVEL)
	_IRQL_requires_same_
	_Function_class_(DRIVER_INITIALIZE)
	DRIVER_INITIALIZE DriverEntry;

};

DRIVER_UNLOAD unload;

HANDLE			g_wfp_filter_engine_handle = NULL;
PDEVICE_OBJECT	g_device_object;
PDEVICE_OBJECT	g_wfp_device_object;

UINT32			g_callout_id;
UINT64			g_filter_id = 0;




VOID unload(_In_ PDRIVER_OBJECT driver_object)
{
	UNREFERENCED_PARAMETER(driver_object);

	net_delete_interface();


	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING symlink = { 0 };


	status = net_wfp_unregister_filter();
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "Failed to unregister filters, status: 0x%x\n", status);
	}

	status = net_wfp_unregister_callout();
	if (!NT_SUCCESS(status)) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "Failed to unregister callout, status: 0x%x\n", status);
	}

	if (g_wfp_filter_engine_handle) {
		FwpmEngineClose(g_wfp_filter_engine_handle);
		g_wfp_filter_engine_handle = NULL;
	}


	IoDeleteDevice(g_wfp_device_object);
	g_wfp_device_object = NULL;

    DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver unloaded\n");
}


_Success_(return == STATUS_SUCCESS)
NTSTATUS net_create_interface(
	__in PDRIVER_OBJECT DriverObject
)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING  nt_device_name;
	UNICODE_STRING  win32_device_name;
	BOOLEAN         symbolic_link = FALSE;

	do
	{

		RtlInitUnicodeString(&nt_device_name, NT_DEVICE_NAME);

		status = IoCreateDevice(DriverObject,
			0,
			&nt_device_name,
			FILE_DEVICE_NETWORK,
			FILE_DEVICE_SECURE_OPEN,
			FALSE,
			&g_device_object);

		DBG_ASSERT(NT_SUCCESS(status));
		if (!NT_SUCCESS(status)){
			break;
		}

		RtlInitUnicodeString(&win32_device_name, DOS_DEVICE_NAME);

		status = IoCreateSymbolicLink(&win32_device_name, &nt_device_name);
		DBG_ASSERT(NT_SUCCESS(status));
		if (!NT_SUCCESS(status)){
			break;
		}
		status = STATUS_SUCCESS;
		symbolic_link = TRUE;

	} while (FALSE);


	if (!NT_SUCCESS(status)) {

		if (g_device_object) {
			KeEnterCriticalRegion();
			IoDeleteDevice(g_device_object);
			g_device_object = NULL;
			KeLeaveCriticalRegion();
		}

		if (symbolic_link) {
			IoDeleteSymbolicLink(&win32_device_name);
			symbolic_link = FALSE;
		}
	}

	return status;
}

_Success_(return == STATUS_SUCCESS)
NTSTATUS net_delete_interface()
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING  win32_device_name;

	DBG_ASSERT(g_device_object);
	if (g_device_object == NULL) {
		return STATUS_UNSUCCESSFUL;
	}

	RtlInitUnicodeString(&win32_device_name, DOS_DEVICE_NAME);

	status = IoDeleteSymbolicLink(&win32_device_name);
	DBG_ASSERT(NT_SUCCESS(status));

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Delete link, win32_device_name:%ws, status:0x%x\r\n", win32_device_name, status);

	IoDeleteDevice(g_device_object);
	g_device_object = NULL;

	return status;
}



NTSTATUS net_wfp_start_filter(_In_ PDRIVER_OBJECT pDriverObject)
{
    FWPM_SERVICE_STATE  engine_status;
	NTSTATUS status = STATUS_SUCCESS;
	FWPM_SESSION wdf_session = { 0 };
	BOOLEAN in_transaction = FALSE;
	BOOLEAN callout_registered = FALSE;



    engine_status = FwpmBfeStateGet();

	DBGPRINT(NWRFLTR_INFO_LEVEL, "engine_status:%d\n", engine_status);

	DBG_ASSERT(engine_status == FWPM_SERVICE_RUNNING);
	if (engine_status != FWPM_SERVICE_RUNNING) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "engine_status != FWPM_SERVICE_RUNNING\n");
		return STATUS_UNSUCCESSFUL;
	}

	wdf_session.flags = FWPM_SESSION_FLAG_DYNAMIC;	
	status = FwpmEngineOpen(NULL, RPC_C_AUTHN_WINNT, NULL, &wdf_session, &g_wfp_filter_engine_handle);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}


	status = FwpmTransactionBegin(g_wfp_filter_engine_handle, 0);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}
	in_transaction = TRUE;



	status = IoCreateDevice(pDriverObject, 0, NULL, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &g_wfp_device_object);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}



	status = net_wfp_register_callout(g_wfp_device_object,&NET_CALLOUT_GUID);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}
	callout_registered = TRUE;

	
	status = net_wfp_register_sublayer();
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}


	status = net_wfp_register_filter();
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}


	status = FwpmTransactionCommit(g_wfp_filter_engine_handle);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}
	in_transaction = FALSE;

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver loaded successfully\n");

__exit:

	if (!NT_SUCCESS(status)) {

		DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver failed to load, status 0x%x\n", status);
		if (in_transaction == TRUE) {
			FwpmTransactionAbort(g_wfp_filter_engine_handle);
			_Analysis_assume_lock_not_held_(g_wfp_filter_engine_handle);
		}
		if (callout_registered == TRUE) {
			net_wfp_unregister_callout();
		}
		status = STATUS_FAILED_DRIVER_ENTRY;
	}

	return status;
}


_Success_(return == STATUS_SUCCESS)
NTSTATUS net_FwpmBfeStateSubscribeChanges()
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
VOID CALLBACK net_subscription_BFE_state_change_callback(
	_Inout_ VOID* context,
	_In_ FWPM_SERVICE_STATE bfe_state)
{
	DBGPRINT(NWRFLTR_INFO_LEVEL, "start\n");

	DBG_ASSERT(context);

	NTSTATUS                status = STATUS_SUCCESS;

	switch (bfe_state)
	{
	case FWPM_SERVICE_RUNNING:
		break;
	case FWPM_SERVICE_STOP_PENDING:
		break;
	}


	DBGPRINT(NWRFLTR_INFO_LEVEL, "status: %#x\n", status);

	return;
}

/*
_Success_(return == STATUS_SUCCESS)
NTSTATUS net_FwpmBfeStateSubscribeChanges()
{
	NTSTATUS status = STATUS_SUCCESS;

	DBGPRINT(NWRFLTR_INFO_LEVEL, "BfeStateSubscribeChanges\n");
	

	return status;
}
*/





_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
_Function_class_(DRIVER_INITIALIZE)
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject,_In_ PUNICODE_STRING pRegistryPath)
{
    DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver loaded\n");

    WPP_INIT_TRACING(pDriverObject, pRegistryPath);

	DBG_NT_ASSERT(pDriverObject);
	DBG_NT_ASSERT(pRegistryPath);


    NTSTATUS    status = STATUS_SUCCESS;

	status = net_create_interface(pDriverObject);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		goto __exit;
	}


	status = net_wfp_start_filter(pDriverObject);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		return status;
	}


	for (UINT i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
        pDriverObject->MajorFunction[i] = net_device_dispatch;
    pDriverObject->DriverUnload = unload;


	/*
	status = IoCreateDevice(pDriverObject, 0, NULL, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &g_wfp_device_object);
	DBG_ASSERT(NT_SUCCESS(status));
	if (!NT_SUCCESS(status)) {
		return status;
	}
	*/


	

__exit:

    if (status != STATUS_SUCCESS) {
        WPP_CLEANUP(pDriverObject);
    }

    DBGPRINT(NWRFLTR_INFO_LEVEL, "Driver end\n");

    return status;
}



NTSTATUS 
net_wfp_register_callout(
	_In_ DEVICE_OBJECT* wdm_device,
	_In_ const GUID* calloutKey
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	FWPS_CALLOUT s_callout = { 0 };
	FWPM_CALLOUT m_callout = { 0 };
	FWPM_DISPLAY_DATA display_data = { 0 };

	if (g_wfp_filter_engine_handle == NULL)
		return STATUS_INVALID_HANDLE;

	display_data.name = NET_CALLOUT_NAME;
	display_data.description = NET_CALLOUT_DESCRIPTION;

	s_callout.calloutKey = *calloutKey;
	s_callout.classifyFn = net_wfp_stub_classify;
	s_callout.notifyFn = net_wfp_stub_notify;
	s_callout.flowDeleteFn = net_wfp_stub_flow_delete;


	status = FwpsCalloutRegister((void*)wdm_device, &s_callout, &g_callout_id);
	if (!NT_SUCCESS(status)) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "Failed to register callout functions for callout, status 0x%x\n", status);
		goto __exit;
	}

	m_callout.calloutKey = *calloutKey;
	m_callout.displayData = display_data;
	m_callout.applicableLayer = FWPM_LAYER_ALE_AUTH_CONNECT_V4;
	m_callout.flags = 0;

	status = FwpmCalloutAdd(g_wfp_filter_engine_handle, &m_callout, NULL, NULL);
	if (!NT_SUCCESS(status)) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "Failed to register example callout, status 0x%x\n", status);
		goto __exit;
	} 

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Callout Registered\n");
	

__exit:

	return status;
}

NTSTATUS 
net_wfp_unregister_callout()
{
	return FwpsCalloutUnregisterById(g_callout_id);
}

NTSTATUS net_wfp_register_sublayer()
{
	NTSTATUS status = STATUS_SUCCESS;
	FWPM_SUBLAYER sublayer = { 0 };

	sublayer.subLayerKey = NET_SUBLAYER_GUID;
	sublayer.displayData.name = NET_SUBLAYER_NAME;
	sublayer.displayData.description = NET_SUBLAYER_DESCRIPTION;
	sublayer.flags = 0;
	sublayer.weight = 0x0f;

	status = FwpmSubLayerAdd(g_wfp_filter_engine_handle, &sublayer, NULL);
	if (!NT_SUCCESS(status)) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "Failed to register sublayer, status 0x%x\n", status);
		return status;
	}

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Sublayer registered\n");

	return status;
}

NTSTATUS net_wfp_register_filter()
{
	NTSTATUS status = STATUS_SUCCESS;
	FWPM_FILTER filter = { 0 };

	filter.displayData.name = NET_FILTER_NAME;
	filter.displayData.description = NET_FILTER_DESCRIPTION;
	filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;	
	filter.subLayerKey = NET_SUBLAYER_GUID;
	filter.weight.type = FWP_UINT8;
	filter.weight.uint8 = 0xf;		
	filter.numFilterConditions = 0;	
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;	
	filter.action.calloutKey = NET_CALLOUT_GUID;


	status = FwpmFilterAdd(g_wfp_filter_engine_handle, &filter, NULL, &(g_filter_id));
	if (!NT_SUCCESS(status)) {
		DBGPRINT(NWRFLTR_INFO_LEVEL, "Failed to register filter, status 0x%x\n", status);
		return status;
	}

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Filter registered\n");

	return status;
}

NTSTATUS 
net_wfp_unregister_filter()
{
	return FwpmFilterDeleteById(g_wfp_filter_engine_handle, g_filter_id);
}


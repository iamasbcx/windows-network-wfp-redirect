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




#define FORMAT_IPV4_ADDR(addr) \
    ((UCHAR *)&(addr))[3], ((UCHAR *)&(addr))[2], ((UCHAR *)&(addr))[1], ((UCHAR *)&(addr))[0]

VOID NTAPI
net_wfp_stub_classify(
	const FWPS_INCOMING_VALUES* inFixedValues,
	const FWPS_INCOMING_METADATA_VALUES* inMetaValues,
	void* layerData,
	const void* classifyContext,
	const FWPS_FILTER* filter,
	UINT64 flowContext,
	FWPS_CLASSIFY_OUT* classifyOut)
{

	UNREFERENCED_PARAMETER(inFixedValues);
	UNREFERENCED_PARAMETER(inMetaValues);
	UNREFERENCED_PARAMETER(layerData);
	UNREFERENCED_PARAMETER(classifyContext);
	UNREFERENCED_PARAMETER(flowContext);
	UNREFERENCED_PARAMETER(filter);

	
	//DBGPRINT(NWRFLTR_INFO_LEVEL, "Stub\n");

	DBG_ASSERT(inFixedValues->layerId == FWPS_LAYER_ALE_AUTH_CONNECT_V4);

	UINT32 local_address = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_LOCAL_ADDRESS].value.uint32;
	UINT32 remote_address = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_REMOTE_ADDRESS].value.uint32;

	UINT16 local_port = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_LOCAL_PORT].value.uint16;
	UINT16 remote_port = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_REMOTE_PORT].value.uint16;

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Connect, %d.%d.%d.%d:%u --> %d.%d.%d.%d:%u\n",
		FORMAT_IPV4_ADDR(local_address), local_port, FORMAT_IPV4_ADDR(remote_address), remote_port);


	classifyOut->actionType = FWP_ACTION_PERMIT;
	return;
}



NTSTATUS 
net_wfp_stub_notify(
	_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType, 
	_In_ const GUID* filterKey, 
	_Inout_ FWPS_FILTER* filter
	)
{
	NTSTATUS status = STATUS_SUCCESS;

	UNREFERENCED_PARAMETER(notifyType);
	UNREFERENCED_PARAMETER(filterKey);
	UNREFERENCED_PARAMETER(filter);

	switch (notifyType) {
	case FWPS_CALLOUT_NOTIFY_ADD_FILTER:
		DBGPRINT(NWRFLTR_INFO_LEVEL, "A new filter has registered callout as its action\n");
		break;
	case FWPS_CALLOUT_NOTIFY_DELETE_FILTER:
		DBGPRINT(NWRFLTR_INFO_LEVEL, "A filter that uses callout has just been deleted\n");
		break;
	}

	return status;
}

VOID NTAPI
net_wfp_stub_flow_delete(
	_In_ UINT16 layerId, 
	_In_ UINT32 calloutId, 
	_In_ UINT64 flowContext
	)
{
	UNREFERENCED_PARAMETER(layerId);
	UNREFERENCED_PARAMETER(calloutId);
	UNREFERENCED_PARAMETER(flowContext);

	DBGPRINT(NWRFLTR_INFO_LEVEL, "Flow delete\n");

}
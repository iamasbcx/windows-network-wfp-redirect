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

#pragma once


extern "C"
{
#pragma warning(push)
#pragma warning(disable: 4201)		// NAMELESS_STRUCT_UNION
#pragma warning(disable: 4324)		// STRUCTURE_PADDED




#include <ntifs.h>                  
#include <ntddk.h>                  
//#include <wdf.h>                  
#include <ndis.h>                   
#include <fwpmk.h>                  
#include <fwpsk.h>                  
#include <netioddk.h>               
#include <ntintsafe.h>              
#include <ntstrsafe.h>              
#include <stdlib.h>                 

#include <guiddef.h>
#include <initguid.h>
#include "ipsectypes.h"

#pragma warning(pop)



#define NT_DEVICE_NAME          L"\\Device\\NetWfpProxy"
#define DOS_DEVICE_NAME         L"\\??\\NetWfpProxy"



#define NET_CALLOUT_NAME			L"ExampleCallout"
#define NET_CALLOUT_DESCRIPTION		L"A callout used for demonstration purposes"

#define NET_SUBLAYER_NAME			L"ExampleSublayer"
#define NET_SUBLAYER_DESCRIPTION	L"A sublayer used to hold filters in an example callout driver"

#define NET_FILTER_NAME				L"ExampleFilter"
#define NET_FILTER_DESCRIPTION		L"A filter that uses the example callout"



// {7BD14A8D-CE5A-4BC0-969F-B3FD41528BDD}
DEFINE_GUID(NET_CALLOUT_GUID,
		0x7bd14a8d, 0xce5a, 0x4bc0, 0x96, 0x9f, 0xb3, 0xfd, 0x41, 0x52, 0x8b, 0xdd);

// {4838D5EA-585C-4187-8D5F-1686459E50F7}
DEFINE_GUID(NET_SUBLAYER_GUID,
		0x4838d5ea, 0x585c, 0x4187, 0x8d, 0x5f, 0x16, 0x86, 0x45, 0x9e, 0x50, 0xf7);

_Success_(return == STATUS_SUCCESS)
NTSTATUS 
net_delete_interface();


NTSTATUS
net_wfp_register_callout(
	_In_ DEVICE_OBJECT* wdm_device,
	_In_ const GUID* calloutKey
	);


NTSTATUS
net_wfp_unregister_callout();

NTSTATUS net_wfp_register_sublayer();

NTSTATUS net_wfp_register_filter();

NTSTATUS
net_wfp_unregister_filter();


}

#include "debug.h"

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


#include <wdm.h>

#include "debug.h"
#include "new.h"
#include "ex_allocate_pool.h"


void* __cdecl operator new(size_t size, POOL_TYPE type)
{
	PVOID pdata = ExAllocatePoolWithTag(type, size, MEM_POOL_TAG);
	if (pdata != NULL) {
		RtlZeroMemory(pdata, size);
	}
	return pdata;
}



void* __cdecl operator new(size_t size, POOL_TYPE type, ULONG tag)
{
    PVOID pdata = ExAllocatePoolWithTag(type, size, tag);
    if (pdata != NULL) {
        RtlZeroMemory(pdata, size);
    }
    return pdata;
}



void* __cdecl operator new[](size_t size, POOL_TYPE type)
{
	PVOID pdata = ExAllocatePoolWithTag(type, size, MEM_POOL_TAG);
	if (pdata != NULL) {
		RtlZeroMemory(pdata, size);
	}
	return pdata;
}

void* __cdecl operator new[](size_t size, POOL_TYPE type, ULONG tag)
{
    PVOID pdata = ExAllocatePoolWithTag(type, size, tag);
	if (pdata != NULL) {
		RtlZeroMemory(pdata, size);
	}
    return pdata;
}


void __cdecl operator delete(void* pdata) noexcept
{
	if (pdata != NULL) {
		ExFreePoolWithTag(pdata, MEM_POOL_TAG);
	}
}


void __cdecl operator delete( void* pdata, ULONG tag) noexcept
{
    if (pdata != NULL) {
        ExFreePoolWithTag(pdata, tag);
    }
}


void __cdecl operator delete[](void* pdata) noexcept
{
	if (pdata != NULL) {
		ExFreePoolWithTag(pdata, MEM_POOL_TAG);
	}
}

void __cdecl operator delete[]( void* pdata, ULONG tag) noexcept
{
    if (pdata != NULL) {
        ExFreePoolWithTag(pdata, tag);
    }
}
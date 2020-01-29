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
/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#include <wdm.h>

/*

// https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/single-binary-opt-in-pool-nx-optin

Add into DriverEntry

#if _WIN32_WINNT >= 0x0A00
	ExInitializeDriverRuntime( DrvRtPoolNxOptIn );
#endif // _WIN32_WINNT >= 0x0A00

*/


#if _WIN32_WINNT >= 0x0A00
#if !POOL_NX_OPTIN
#error "POOL_NX_OPTIN is not defined. Set POOL_NX_OPTIN=1"
#endif
#endif


PVOID
ex_allocate_pool(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes
)
{
#if _WIN32_WINNT >= 0x0A00
	if (NonPagedPool == pool_type)
		pool_type = (POOL_TYPE)NonPagedPoolNx;
#endif

#if _WIN32_WINNT < 0x0A00
#undef ExAllocatePool
#endif
	return ExAllocatePool(pool_type, number_of_bytes);
}


PVOID
ex_allocate_pool_with_quota_tag(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes,
	_In_ ULONG tag
)
{
#if _WIN32_WINNT >= 0x0A00
	if (NonPagedPool == pool_type)
		pool_type = (POOL_TYPE)NonPagedPoolNx;
#endif

#if _WIN32_WINNT < 0x0A00
#undef ExAllocatePoolWithQuotaTag
#endif
	return ExAllocatePoolWithQuotaTag(pool_type, number_of_bytes, tag);
}


PVOID
ex_allocate_pool_with_quota(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes
	)
{
#if _WIN32_WINNT >= 0x0A00
	if (NonPagedPool == pool_type)
		pool_type = (POOL_TYPE)NonPagedPoolNx;
#endif

#if _WIN32_WINNT < 0x0A00
#undef ExAllocatePoolWithQuota
#endif
	return ExAllocatePoolWithQuota(pool_type, number_of_bytes);
}


VOID
ex_free_pool(
	_In_ PVOID p
	)
{
#if _WIN32_WINNT < 0x0A00
#undef ExFreePool
#endif
	ExFreePool(p);
}


PVOID
ex_allocate_pool_with_tag(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes,
	_In_ ULONG tag
)
{
#if _WIN32_WINNT >= 0x0A00
	if (NonPagedPool == pool_type)
		pool_type = (POOL_TYPE)NonPagedPoolNx;
#endif

#if _WIN32_WINNT < 0x0A00
#undef ExAllocatePoolWithTag
#endif
	return ExAllocatePoolWithTag(pool_type, number_of_bytes, tag);
}


VOID
ex_free_pool_with_tag(
	_In_ PVOID p,
	_In_ ULONG tag
	)
{
#if _WIN32_WINNT < 0x0A00
#undef ExFreePoolWithTag
#endif

#if _WIN32_WINNT == 0x0400 //NT4
	ExFreePool(P);
#else
	ExFreePoolWithTag(p, tag);
#endif
}
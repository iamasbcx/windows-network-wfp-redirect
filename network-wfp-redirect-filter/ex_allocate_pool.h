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


extern
PVOID
ex_allocate_pool(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes
);

extern
PVOID
ex_allocate_pool_with_tag(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes,
	_In_ ULONG tag
);

extern
PVOID
ex_allocate_pool_with_quota_tag(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes,
	_In_ ULONG tag
);

extern
PVOID
ex_allocate_pool_with_quota(
	_In_ POOL_TYPE pool_type,
	_In_ SIZE_T number_of_bytes
);

extern
VOID
ex_free_pool(
	_In_ PVOID p
);

extern
VOID
ex_free_pool_with_tag(
	_In_ PVOID p,
	_In_ ULONG tag
);



#ifdef _AMD64_

#undef ExAllocatePool
#define ExAllocatePool(a,b) ex_allocate_pool_with_tag(a,b,MEM_POOL_TAG)

#undef ExAllocatePoolWithTag
#define ExAllocatePoolWithTag(a,b,c) ex_allocate_pool_with_tag(a,b,c)

#undef ExAllocatePoolWithQuota
#define ExAllocatePoolWithQuota(a,b) ex_allocate_pool_with_quota_tag(a,b,MEM_POOL_TAG)

#undef ExFreePool
#define ExFreePool(a) ex_free_pool_with_tag(a,MEM_POOL_TAG)

#undef ExFreePoolWithTag
#define ExFreePoolWithTag(a,b) ex_free_pool_with_tag(a,b)

#else// ! _AMD64_

#undef ExAllocatePool
#define ExAllocatePool(a,b)  ex_allocate_pool(a,b)

#undef ExAllocatePoolWithTag
#define ExAllocatePoolWithTag(a,b,c) ex_allocate_pool_with_tag(a,b,c)

#undef ExAllocatePoolWithQuota
#define ExAllocatePoolWithQuota(a,b) ex_allocate_pool_with_quota(a,b)

#undef ExFreePool
#define ExFreePool(a) ex_free_pool(a)

#undef ExFreePoolWithTag
#define ExFreePoolWithTag(a,b) ex_free_pool_with_tag(a,b)

#endif
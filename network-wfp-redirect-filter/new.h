/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#pragma once

#define MEM_POOL_TAG ((ULONG) '++CS')


void* __cdecl operator new(size_t size, POOL_TYPE type, ULONG tag = MEM_POOL_TAG);
void* __cdecl operator new[](size_t size, POOL_TYPE type, ULONG tag = MEM_POOL_TAG);

void __cdecl operator delete(void* p);
void __cdecl operator delete(void* p, ULONG tag);
void __cdecl operator delete[](void* p);
void __cdecl operator delete[](void* p, ULONG tag);

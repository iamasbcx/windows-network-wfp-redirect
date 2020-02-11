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

#define MEM_POOL_TAG ((ULONG) '++CS')


void* __cdecl operator new(size_t size, POOL_TYPE type, ULONG tag = MEM_POOL_TAG);
void* __cdecl operator new[](size_t size, POOL_TYPE type, ULONG tag = MEM_POOL_TAG);

void __cdecl operator delete(void* p);
void __cdecl operator delete(void* p, ULONG tag);
void __cdecl operator delete[](void* p);
void __cdecl operator delete[](void* p, ULONG tag);

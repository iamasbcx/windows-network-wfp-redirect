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

#include <stdarg.h>
#include <stdio.h>

#define NWRFLTR_ERROR_LEVEL		0
#define NWRFLTR_WARNING_LEVEL	1
#define NWRFLTR_TRACE_LEVEL		2
#define NWRFLTR_INFO_LEVEL		3
#define NWRFLTR_MASK			0x80000000



#define WPP_INIT_TRACING(DriverObject, RegistryPath)
#define WPP_CLEANUP(DriverObject)


void dbg_print(
	__in    ULONG Level,
	__in    const char* FuncName,
	__in    const char* fmt, ...
	);

#if DBG

#define DBGPRINT(level, fmt, ...)		dbg_print(level, __FUNCTION__, fmt, __VA_ARGS__)

#define DBG_ASSERT( _X_ ) ASSERT( _X_ )
#define DBG_NT_ASSERT( _X_ ) NT_ASSERT( _X_ )

#else

DBGPRINT((_X_))
#define DBG_ASSERT( _X_ )
#define DBG_NT_ASSERT( _X_ )
#endif
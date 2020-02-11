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





void dbg_print(
	__in    ULONG Level,
	__in    const char* FuncName,
	__in    const char* fmt, ...
	)
{
	UNREFERENCED_PARAMETER(Level);
	UNREFERENCED_PARAMETER(FuncName);
	UNREFERENCED_PARAMETER(fmt);

	va_list args;
	va_start(args, fmt);

	DbgPrint(fmt, args);

	va_end(args);


}
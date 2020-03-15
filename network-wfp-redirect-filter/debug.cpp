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

#include <ntddk.h>


#include <stdarg.h>
#include <stdio.h>
#include <Ntstrsafe.h>


#include "debug.h"


#define LOG_MSG_SIZE 1024

#define TWO_SECONDS (2*1000*1000*10)
#define ALMOST_TWO_SECONDS (TWO_SECONDS - 1)


void dbg_print(
	__in    ULONG level,
	__in    const char* func_name,
	__in    const char* fmt, ...
	)
{
	UNREFERENCED_PARAMETER(level);
	UNREFERENCED_PARAMETER(func_name);
	UNREFERENCED_PARAMETER(fmt);

	NTSTATUS status = STATUS_SUCCESS;
	char msg[LOG_MSG_SIZE];
	LARGE_INTEGER time;
	TIME_FIELDS time_fields;

	va_list args;

	va_start(args, fmt);


	status = RtlStringCbVPrintfA(msg, LOG_MSG_SIZE, fmt, args);
	if (!NT_SUCCESS(status)) {
		msg[sizeof(msg) - 1] = '\0';
	}
	

	KeQuerySystemTime(&time);
	ExSystemTimeToLocalTime(&time, &time);
	//KernelTimeToSystemTime(&time2, &time3);


	time.QuadPart = time.QuadPart + ALMOST_TWO_SECONDS;

	(VOID)RtlTimeToTimeFields(&time, &time_fields);


	DbgPrint(msg);

	va_end(args);


}
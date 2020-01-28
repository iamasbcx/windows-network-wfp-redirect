/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

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
/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

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

#else

DBGPRINT((_X_))

#endif
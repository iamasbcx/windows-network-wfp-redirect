/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */



#pragma once

#include <iostream>
#include <string>
#include <time.h>

namespace util
{
	// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
	const std::string current_datetime()
	{
		time_t     now = time(NULL);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
		return buf;
	}



	const std::string get_only_file_name_a(HMODULE hModule)
	{
		char     path[_MAX_PATH] = { 0 };
		char     fname[_MAX_FNAME] = { 0 };

		// Get EXE-module path and file name
		GetModuleFileNameA(hModule, path, sizeof(path) / sizeof(char));

		// Replace file name and extension with our mask
		_splitpath_s(path, NULL, 0, NULL, 0, fname, sizeof(fname) / sizeof(char), NULL, 0);

		return fname;
	}


	const std::wstring get_only_file_name_w(HMODULE hModule)
	{
		wchar_t     path[_MAX_PATH] = { 0 };
		wchar_t     fname[_MAX_FNAME] = { 0 };

		// Get EXE-module path and file name
		GetModuleFileNameW(hModule, path, sizeof(path) / sizeof(wchar_t));

		// Replace file name and extension with our mask
		_wsplitpath_s(path, NULL, 0, NULL, 0, fname, sizeof(fname) / sizeof(wchar_t), NULL, 0);

		return fname;
	}


}
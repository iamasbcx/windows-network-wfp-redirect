#include <windows.h>
#include <tchar.h>
#include <DbgHelp.h>

#include <iostream>

using namespace std;

#pragma comment (lib, "dbghelp.lib")

void write_full_dump(HANDLE hproc)
{
	const DWORD flags = MiniDumpWithFullMemory |
		MiniDumpWithFullMemoryInfo |
		MiniDumpWithHandleData |
		MiniDumpWithUnloadedModules |
		MiniDumpWithThreadInfo;

	HANDLE hfile = CreateFile(L"memory.dmp", GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!hfile) {
		std::wcerr << L"Failed to write dump: Invalid dump file";

	} else {
		BOOL Result = MiniDumpWriteDump(hproc,GetProcessId(hproc), hfile,(MINIDUMP_TYPE)flags,nullptr,nullptr,nullptr);

		CloseHandle(hfile);

		if (!Result){
			std::wcerr << L"Looks like an error: MiniDumpWriteDump failed";
		}
	}// End if

	return;
}

/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#include <windows.h>
#include <iostream>
#include <string.h>

#include "input_cmd.h"
#include "../network-wfp-redirect-filter/kernel_to_user.h"


using namespace std;



void usage() {



}



void test()
{
	HANDLE	driver;
	NET_IO_REQUEST request = { 0 };
	DWORD bytes_sent;

	driver = CreateFile(L"\\\\.\\NetWfpRdr",GENERIC_ALL, 0, NULL, OPEN_EXISTING, 0x0, NULL);
	if (INVALID_HANDLE_VALUE == driver) {
		wprintf(L"Can't open driver %u\n", GetLastError());
		return;
	}

	request.test = 1;

	DeviceIoControl(driver, IOCTL_NET_FILTER_TEST,&request, sizeof(request), NULL, 0x0,  &bytes_sent, NULL);

	WaitForSingleObject(driver, INFINITE);
	CloseHandle(driver);
}


int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

	CInputCmd input(argc, argv);
	if (input.cmd_option_exists("-h")) {
		usage();
	}


	test();

}



/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */


#include <iostream>
#include <string.h>

#include "input_cmd.h"

using namespace std;


void usage() {



}


int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

	CInputCmd input(argc, argv);
	if (input.cmd_option_exists("-h")) {
		usage();
	}
}



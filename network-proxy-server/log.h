/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#pragma once

#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
#include <time.h>


using namespace std;


#define LOG CLog::get_log()

class CLog
{
public:

    void log(const std::string& sMessage);


    void log(const char* format, ...);
    
    CLog& operator<<(const string& sMessage);

    static CLog* get_log();
private:

    CLog();

    CLog(const CLog&) {};             // copy constructor is private

    CLog& operator=(const CLog&) { return *this; };  // assignment operator is private

    static const std::string m_sFileName;

    static CLog* m_pThis;

    static ofstream m_Logfile;
};



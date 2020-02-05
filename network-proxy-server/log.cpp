/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#include <windows.h>

#include "log.h"
#include "utilities.h"


const string CLog::m_sFileName = "log.txt";

CLog* CLog::m_pThis = NULL;

ofstream CLog::m_Logfile;


CLog::CLog()
{

}


CLog* CLog::get_log() {

	if (m_pThis == NULL) {
		m_pThis = new CLog();

		string file_name = util::get_only_file_name_a(NULL);

		file_name.append(".log");

		m_Logfile.open(file_name.c_str(), ios::out | ios::app);
	}
	return m_pThis;
}

void CLog::log(const char* format, ...)
{
	char* sMessage = NULL;
	int nLength = 0;
	va_list args;
	va_start(args, format);

	nLength = _vscprintf(format, args) + 1;
	sMessage = new char[nLength];
	vsprintf_s(sMessage, nLength, format, args);
	//vsprintf(sMessage, format, args);

	m_Logfile << util::current_datetime() << ":\t";
	m_Logfile << sMessage << "\n";
	va_end(args);

	delete[] sMessage;
}

void CLog::log(const string& sMessage)
{
	m_Logfile << util::current_datetime() << ":\t";
	m_Logfile << sMessage << "\n";
}

CLog& CLog::operator<<(const string& sMessage)
{
	m_Logfile << "\n" << util::current_datetime() << ":\t";
	m_Logfile << sMessage << "\n";
	return *this;
}
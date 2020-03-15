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
#include <ntstrsafe.h>   

#include <stdarg.h>
#include <stdio.h>
#include <Ntstrsafe.h>



#define TICKSPERMIN        600000000   
#define TICKSPERSEC        10000000   
#define TICKSPERMSEC       10000   
#define SECSPERDAY         86400   
#define SECSPERHOUR        3600   
#define SECSPERMIN         60   
#define MINSPERHOUR        60   
#define HOURSPERDAY        24   
#define EPOCHWEEKDAY       0   
#define DAYSPERWEEK        7   
#define EPOCHYEAR          1601   
#define DAYSPERNORMALYEAR  365   
#define DAYSPERLEAPYEAR    366   
#define MONSPERYEAR        12   

static const int YearLengths[2] = { DAYSPERNORMALYEAR, DAYSPERLEAPYEAR };
static const int MonthLengths[2][MONSPERYEAR] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static int  IsLeapYear(int Year);

/*
void
KernelTimeToSystemTime(PLARGE_INTEGER KernelTime, SYSTEMTIME* lpSystemTime)
{
	const int* Months;
	int LeapSecondCorrections, SecondsInDay, CurYear;
	int LeapYear, CurMonth;
	long Days;
	__int64 Time = KernelTime->QuadPart;

	// Extract millisecond from time and convert time into seconds 
	lpSystemTime->wMilliseconds = (USHORT)((Time % TICKSPERSEC) / TICKSPERMSEC);
	Time = Time / TICKSPERSEC;

	LeapSecondCorrections = 0;

	// Split the time into days and seconds within the day 
	Days = (long)(Time / SECSPERDAY);
	SecondsInDay = (long)(Time % SECSPERDAY);

	// Adjust the values for GMT and leap seconds 
	SecondsInDay += LeapSecondCorrections;
	while (SecondsInDay < 0) {
		SecondsInDay += SECSPERDAY;
		Days--;
	}
	while (SecondsInDay >= SECSPERDAY) {
		SecondsInDay -= SECSPERDAY;
		Days++;
	}

	// compute time of day 
	lpSystemTime->wHour = (USHORT)(SecondsInDay / SECSPERHOUR);
	SecondsInDay = SecondsInDay % SECSPERHOUR;
	lpSystemTime->wMinute = (USHORT)(SecondsInDay / SECSPERMIN);
	lpSystemTime->wSecond = (USHORT)(SecondsInDay % SECSPERMIN);

	// compute day of week
	lpSystemTime->wDayOfWeek = (USHORT)((EPOCHWEEKDAY + Days) % DAYSPERWEEK);

	// compute year 
	CurYear = EPOCHYEAR;

	for (;;) {
		LeapYear = IsLeapYear(CurYear);
		if (Days < (long)YearLengths[LeapYear])
			break;
		CurYear++;
		Days = Days - (long)YearLengths[LeapYear];
	}
	lpSystemTime->wYear = (USHORT)CurYear;

	// Compute month of year 
	Months = MonthLengths[LeapYear];
	for (CurMonth = 0; Days >= (long)Months[CurMonth]; CurMonth++)
		Days = Days - (long)Months[CurMonth];
	lpSystemTime->wMonth = (USHORT)(CurMonth + 1);
	lpSystemTime->wDay = (USHORT)(Days + 1);
}
*/

/*
int
IsLeapYear(int Year)
{
	return Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0) ? 1 : 0;
}
*/


ULONG get_current_time()
{
	LARGE_INTEGER current_system_time;
	LARGE_INTEGER current_local_time;
	ULONG time = 0;

	KeQuerySystemTime(&current_system_time);
	ExSystemTimeToLocalTime(&current_system_time, &current_local_time);

	//RtlTimeToSecondsSince1970(&current_local_time, &time);

	return time;
}

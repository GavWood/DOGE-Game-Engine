////////////////////////////////////////////////////////////////////////////////
// BtTime.cpp

#include "BtTime.h"
#include "BtBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys\timeb.h>
#include <time.h>
#include <windows.h>
#include <time.h>

BtFloat m_start = 0;
LARGE_INTEGER m_depart;

////////////////////////////////////////////////////////////////////////////////
// Init

void BtTime::Init()
{
	QueryPerformanceCounter(&m_depart);
}

////////////////////////////////////////////////////////////////////////////////
// Statics

BtFloat BtTime::m_tick = 1.0f / 60.0f;

////////////////////////////////////////////////////////////////////////////////
// GetTime

BtU32 BtTime::GetTime()
{
	clock_t currentTime;
	currentTime = clock();

	return (BtU32)currentTime;
}

////////////////////////////////////////////////////////////////////////////////
// GetTick

BtFloat BtTime::GetTick()
{
	return m_tick;
}

////////////////////////////////////////////////////////////////////////////////
// SetTick

void BtTime::SetTick(BtFloat tick)
{
	m_tick = tick;
}

////////////////////////////////////////////////////////////////////////////////
// GetElapsedTimeInSeconds

BtFloat BtTime::GetElapsedTimeInSeconds()
{
	LARGE_INTEGER now;
	LARGE_INTEGER freq;

	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);

	return (now.QuadPart - m_depart.QuadPart) / static_cast<float>(freq.QuadPart);
}

////////////////////////////////////////////////////////////////////////////////
// GetElapsedTimeInMilliseconds

BtFloat BtTime::GetElapsedTimeInMilliseconds()
{
	return BtTime::GetElapsedTimeInSeconds() * 1000.0f;
}

////////////////////////////////////////////////////////////////////////////////
// GetDay

BtU32 BtTime::GetDay()
{
	struct tm newtime;

	time_t systemTime;

	time( &systemTime );

	localtime_s( &newtime, &systemTime );

	return newtime.tm_mday;
}

////////////////////////////////////////////////////////////////////////////////
// GetMonth

BtU32 BtTime::GetMonth()
{
	struct tm newtime;

	time_t systemTime;

	time( &systemTime );

	localtime_s( &newtime, &systemTime );

	return newtime.tm_mon + 1;

}

////////////////////////////////////////////////////////////////////////////////
// GetYear

BtU32 BtTime::GetYear()
{
	struct tm newtime;

	time_t systemTime;

	time( &systemTime );

	localtime_s( &newtime, &systemTime );

	return newtime.tm_year + 1900;
}

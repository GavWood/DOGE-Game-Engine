////////////////////////////////////////////////////////////////////////////////
// BtTimeDX11.cpp

#ifdef WIN32
#include "BtTimeDX11.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys\timeb.h>

BtFloat BtTimeDX11::m_start = 0;
LARGE_INTEGER m_depart;

////////////////////////////////////////////////////////////////////////////////
// Init

void BtTimeDX11::Init()
{
	QueryPerformanceCounter(&m_depart);
}

////////////////////////////////////////////////////////////////////////////////
// GetElapsedTimeInSeconds

BtFloat BtTimeDX11::GetElapsedTimeInSeconds()
{
	LARGE_INTEGER now;
	LARGE_INTEGER freq;

	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);

	return (now.QuadPart - m_depart.QuadPart) / static_cast<float>(freq.QuadPart);
}

////////////////////////////////////////////////////////////////////////////////
// Day

BtU32 BtTimeDX11::Day()
{
	struct tm newtime;

	time_t systemTime;

	time( &systemTime );

	localtime_s( &newtime, &systemTime );

	return newtime.tm_mday;
}

////////////////////////////////////////////////////////////////////////////////
// Month

BtU32 BtTimeDX11::Month()
{
	struct tm newtime;

	time_t systemTime;

	time( &systemTime );

	localtime_s( &newtime, &systemTime );

	return newtime.tm_mon + 1;

}

////////////////////////////////////////////////////////////////////////////////
// Year

BtU32 BtTimeDX11::Year()
{
	struct tm newtime;

	time_t systemTime;

	time( &systemTime );

	localtime_s( &newtime, &systemTime );

	return newtime.tm_year + 1900;
}
#endif


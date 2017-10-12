////////////////////////////////////////////////////////////////////////////////
// BtTime.cpp

#include "BtTime.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

struct timeval start, end;
long mtime, seconds, useconds;

////////////////////////////////////////////////////////////////////////////////
// Init

void BtTime::Init()
{
    gettimeofday(&start, NULL);
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
// GetElapsedTimeInMilliseconds

BtFloat BtTime::GetElapsedTimeInMilliseconds()
{
    gettimeofday(&end, NULL);
    
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    
    return mtime;
}

////////////////////////////////////////////////////////////////////////////////
// GetElapsedTimeInSeconds

BtFloat BtTime::GetElapsedTimeInSeconds()
{
    gettimeofday(&end, NULL);
    
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    
    return mtime / 1000.0f;
}

////////////////////////////////////////////////////////////////////////////////
// GetDay

BtU32 BtTime::GetDay()
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetMonth

BtU32 BtTime::GetMonth()
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetYear

BtU32 BtTime::GetYear()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// HlTimer

// Includes
#include "HlTimer.h"
#include "BtTime.h"
#include "BtString.h"
#include "ApConfig.h"

////////////////////////////////////////////////////////////////////////////////
// IsReady

//static
BtBool HlTimer::IsReady()
{
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();

	if (currentTime > m_start + m_duration)
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// ResetTimer

void HlTimer::ResetTimer( BtFloat duration)
{
	m_start = BtTime::GetElapsedTimeInSeconds();
	m_duration = duration;
}

////////////////////////////////////////////////////////////////////////////////
// GetElapsedTime

//static
BtFloat HlTimer::GetElapsedTime()
{
	BtFloat currentTime = BtTime::GetElapsedTimeInSeconds();
	return currentTime - m_start;
}

////////////////////////////////////////////////////////////////////////////////
// GetStartTime

//static
BtFloat HlTimer::GetStartTime()
{
	return m_start;
}

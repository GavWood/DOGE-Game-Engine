////////////////////////////////////////////////////////////////////////////////
// BtThread.cpp

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include "BtThread.h"

////////////////////////////////////////////////////////////////////////////////
// Forward declarations

DWORD WINAPI LBtThread_EntryPoint( LPVOID lpParameter );

////////////////////////////////////////////////////////////////////////////////
// Constructor

BtThread::BtThread()
{
	m_nThreadID = 0;
	m_bThreadEnded = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Start

void BtThread::Start()
{
	DWORD Flags = 0;

	Flags = CREATE_SUSPENDED;

	m_nThreadHandle = (BtU32) CreateThread( BtNull,
											0,
											LBtThread_EntryPoint,
											static_cast< void* >( this ),
											Flags,
											(LPDWORD) &m_nThreadID );

	ResumeThread( (HANDLE)m_nThreadHandle );
}

////////////////////////////////////////////////////////////////////////////////
// End

void BtThread::End()
{
	// Call the closing event so the thread knows it's been told to close
	Closing();

	// Set the thread to inactive
	IsEnded( BtTrue );

	// We have to resume the thread for it to end it's loop
	Resume();

	// Wait for the thread to finish. It can now
	WaitForSingleObject( (HANDLE) m_nThreadHandle, INFINITE );
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

void BtThread::Resume()
{
	ResumeThread( (HANDLE) m_nThreadHandle );
}

////////////////////////////////////////////////////////////////////////////////
// Suspend

void BtThread::Suspend()
{
	SuspendThread( (HANDLE) m_nThreadHandle );
}

////////////////////////////////////////////////////////////////////////////////
// YieldThread

void BtThread::YieldThread()
{
	SleepThread(0);
}

////////////////////////////////////////////////////////////////////////////////
// SleepThread

void BtThread::SleepThread( BtU32 milliseconds )
{
	Sleep( milliseconds );
}

////////////////////////////////////////////////////////////////////////////////
// LBtThread_EntryPoint

DWORD WINAPI LBtThread_EntryPoint( LPVOID lpParameter )
{
	BtThread* pThread = reinterpret_cast< BtThread* >( lpParameter );

	pThread->Create();

	pThread->IsEnded( BtFalse );

	// Update the threading function
	while( pThread->IsEnded() == BtFalse )
	{
		pThread->Update();
	}

	pThread->Destroy();
	
	// Exit the thread
	ExitThread(0);

	// Return success
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Priority

void BtThread::Priority( eThreadPriority ePriority )
{
	m_ePriority = ePriority;

	switch( m_ePriority )
	{
	case TP_Low:
		SetThreadPriority( (HANDLE) m_nThreadHandle, THREAD_PRIORITY_LOWEST );
		break;
	case TP_Normal:
		SetThreadPriority( (HANDLE) m_nThreadHandle, THREAD_PRIORITY_NORMAL );
		break;
	case TP_High:
		SetThreadPriority( (HANDLE) m_nThreadHandle, THREAD_PRIORITY_TIME_CRITICAL );
		break;
	}
}
#endif

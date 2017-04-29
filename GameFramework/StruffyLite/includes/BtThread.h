////////////////////////////////////////////////////////////////////////////////
// BtThread.h

#pragma once
#include "BtTypes.h"

enum eThreadPriority
{
	TP_Low = 0,
	TP_Normal,
	TP_High
};

class BtThread
{
public:

	// Constructor
	BtThread();
	
	virtual void				Create() = 0;
	virtual void				Destroy() = 0;
	virtual void				Update() = 0;

	// Public functions
	void						Start();
	void						End();
	void						Resume();
	void						Suspend();
	void						YieldThread();
	void						SleepThread( BtU32 milliseconds );

	// Accessors
	void						Priority( eThreadPriority Priority );
	BtBool						IsEnded();
	void						IsEnded( BtBool active );
	virtual void				Closing();

	BtU32						ThreadHandle();
	BtU32						ThreadID();
	BtU32						Priority();

 private:
	
	BtU32						m_nThreadID;
	BtU32						m_nThreadHandle;
	eThreadPriority				m_ePriority;

	volatile BtBool				m_bThreadEnded;
};

////////////////////////////////////////////////////////////////////////////////
// ThreadID

inline BtU32 BtThread::ThreadID()
{
	return m_nThreadID;
}

////////////////////////////////////////////////////////////////////////////////
// ThreadHandle

inline BtU32 BtThread::ThreadHandle()
{
	return m_nThreadHandle;
}

////////////////////////////////////////////////////////////////////////////////
// Priority

inline BtU32 BtThread::Priority()
{
	return m_ePriority;
}

////////////////////////////////////////////////////////////////////////////////
// bActive

inline void BtThread::IsEnded( BtBool isEnded )
{
	m_bThreadEnded = isEnded;
}

////////////////////////////////////////////////////////////////////////////////
// bActive

inline BtBool BtThread::IsEnded()
{
	return m_bThreadEnded;
}

////////////////////////////////////////////////////////////////////////////////
// Closing

inline void BtThread::Closing()
{
	// This is a virtual function called to indicate this thread is closing
}

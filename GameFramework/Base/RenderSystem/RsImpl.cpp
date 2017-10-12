////////////////////////////////////////////////////////////////////////////////
// RsImpl.cpp

// Includes
#include "RsImpl.h"
#include "BtMemory.h"
#include "BtTime.h"
#include "BtPrint.h"
#include "ApConfig.h"

//static
RsImpl *RsImpl::m_pInstance = BtNull;

const BtU32 RenderHeapSize = 1024 * 100;

////////////////////////////////////////////////////////////////////////////////
// Constructor

RsImpl::RsImpl()
{
	m_frameNum = 0;
	m_lastFrameNum = 0;
	m_width = 800;
	m_height = 600;
	m_isFullScreen = BtFalse;
	m_pInstance = this;
	m_orientation = RsOrientation_Landscape;
}

////////////////////////////////////////////////////////////////////////////////
// Create

void RsImpl::Create()
{
	// Allocate from the heap
	m_pHeap = BtMemory::Allocate( RenderHeapSize );
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void RsImpl::Destroy()
{
	// Free the heap memory
	BtMemory::Free( m_pHeap );
}

////////////////////////////////////////////////////////////////////////////////
// AllocateGraphicsMemory

BtU8 *RsImpl::AllocateGraphicsMemory( BtU32 size )
{
	// Calculate the current heap position
	BtU8 *pMemory = m_pHeap + m_runTimeHeap;

	// Advance the heap
	m_runTimeHeap += size;

	// Return the current pointer
	return pMemory;
}

////////////////////////////////////////////////////////////////////////////////
// pInstance

//static
RsImpl *RsImpl::pInstance()
{
	return m_pInstance;
}

////////////////////////////////////////////////////////////////////////////////
// BeginScene

void RsImpl::BeginScene()
{
	m_runTimeHeap = 0;
}

////////////////////////////////////////////////////////////////////////////////
// EndScene

void RsImpl::EndScene()
{
	++m_frameNum;

	BtFloat currentTime = BtTime::GetElapsedTimeInMilliseconds();//(BtFloat)BtTime::GetTime();

	// Update the scene stats once per second
	if( ( m_frameNum - m_lastFrameNum ) > 100 )
	{
		m_fps = (BtFloat) 100 / (currentTime - m_lastTime) * 1000.0f;
		m_lastTime = currentTime;
		m_frameNum = 0;

		m_lastFrameNum = m_frameNum;
	}
}

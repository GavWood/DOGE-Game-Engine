////////////////////////////////////////////////////////////////////////////////
// RsImpl.h

#pragma once
#include "BtTypes.h"
#include "BtLinkedList.h"
#include "RsRenderTarget.h"
#include "RsCaps.h"
#include "RsFont.h"
#include "RsUtil.h"
#include "RsVertex.h"
#include "RsShader.h"

const BtU32 MaxVertex = 65535;
const BtU32 MaxIndex = 65535;

// Class declaration
class RsImpl
{
public:

	// Constructor
	RsImpl();

	virtual void						   *GetHandle() = 0;

	virtual void							Create();
	virtual void							Destroy();

	BtU8								   *AllocateGraphicsMemory( BtU32 size );

	virtual RsRenderTarget				   *GetNewRenderTarget() = 0;

	virtual void							EmptyRenderTargets() = 0;

	virtual void							BeginScene();
	virtual void							Render() = 0;
	virtual void							EndScene();

	// Accessors
	static RsImpl						   *pInstance();

	BtFloat									GetFPS();
	MtVector2 								GetScreenCenter();
	MtVector2 								GetDimension();
	void									SetDimension( const MtVector2& v2Dimension );

	void									SetFullScreen( BtBool isFullScreen );
	BtBool									IsFullScreen();

	RsOrientation							GetOrientation();
	void									SetOrientation( RsOrientation orientation );

	virtual RsCaps						   *GetCaps() = 0;

	BtU32									GetWidth();
	BtU32									GetHeight();

	BtU32									GetRefreshRate();

	BtU32									GetCurrentVertex();

protected:

	static RsImpl						   *m_pInstance;

	BtU32									m_width;
	BtU32									m_height;

	RsOrientation							m_orientation;

	BtFloat									m_fps;
	BtFloat									m_lastTime;
	BtU32									m_frameNum;
	BtU32									m_lastFrameNum;
	MtVector2								m_v2ScreenDimension;

	RsRenderTarget						   *m_pCurrentRenderTarget;
	RsShader							   *m_pCurrentShader;

	// Vertex
	BtArray<RsVertex3, MaxVertex>			m_vertex;
	BtArray<BtU32, MaxIndex>				m_index;
	BtU32									m_currentVertex;
	BtU32									m_currentIndex;

	// Graphics heap memory
	BtU8								   *m_pHeap;
	BtU32									m_runTimeHeap;

	BtBool									m_isFullScreen;
};

////////////////////////////////////////////////////////////////////////////////
// GetWidth

inline BtU32 RsImpl::GetWidth()
{
	return m_width;
}

////////////////////////////////////////////////////////////////////////////////
// GetHeight

inline BtU32 RsImpl::GetHeight()
{
	return m_height;
}

////////////////////////////////////////////////////////////////////////////////
// GetRefreshRate

inline BtU32 RsImpl::GetRefreshRate()
{
	return 60;
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

inline MtVector2 RsImpl::GetDimension()
{
	return MtVector2( (BtFloat)m_width, (BtFloat)m_height );
}

////////////////////////////////////////////////////////////////////////////////
// GetOrientation

inline RsOrientation RsImpl::GetOrientation()
{
	return m_orientation;
}

////////////////////////////////////////////////////////////////////////////////
// SetOrientation

inline void RsImpl::SetOrientation( RsOrientation orientation )
{
	m_orientation = orientation;
}

////////////////////////////////////////////////////////////////////////////////
// GetFPS

inline BtFloat RsImpl::GetFPS()
{
	return m_fps;
}

////////////////////////////////////////////////////////////////////////////////
// GetScreenCenter

inline MtVector2 RsImpl::GetScreenCenter()
{
	return GetDimension() * 0.5f;
}

////////////////////////////////////////////////////////////////////////////////
// GetCurrentVertex

inline BtU32 RsImpl::GetCurrentVertex()
{
	return m_currentVertex;
}

////////////////////////////////////////////////////////////////////////////////
// SetDimension

inline void RsImpl::SetDimension( const MtVector2 &v2Dimension )
{
	m_width  = (BtU32)v2Dimension.x;
	m_height = (BtU32)v2Dimension.y;
}

//static
inline void RsImpl::SetFullScreen( BtBool isFullScreen )
{
	m_isFullScreen = isFullScreen;
}

//static
inline BtBool RsImpl::IsFullScreen()
{
	return m_isFullScreen;
}

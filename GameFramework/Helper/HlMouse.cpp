////////////////////////////////////////////////////////////////////////////////
// HlMouse.cpp

// Includes
#include "BaArchive.h"
#include "HlMouse.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "RsUtil.h"
#include "ShTouch.h"
#include "ShHMD.h"
#include "RsSprite.h"
#include "ApConfig.h"
#include "HlDraw.h"
#include "UiKeyboard.h"

//static
RsSprite *HlMouse::m_pSprite = BtNull;
MtVector2 HlMouse::m_v2Position[32];
BtBool HlMouse::m_isVisible[32];
BtFloat HlMouse::m_scalar = 1.0f;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlMouse::Setup( BaArchive *pUtilityArchive )
{
	m_pSprite = pUtilityArchive->GetSprite("MousePointers");
	//BtAssert( m_pSprite );
}

////////////////////////////////////////////////////////////////////////////////
// SetScalar

void HlMouse::SetScalar( BtFloat s )
{
	m_scalar = s;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlMouse::Render()
{
	Render( RsUtil::GetDimension() );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlMouse::Render( MtVector2 v2Dimension )
{
	// The dimension is passed into this function so we can render to rendertarget other than the screen
	if( ShHMD::IsHMD() )
	{
		return;
	}
	
	BtFloat width = RsUtil::GetDimension().x;
	BtFloat height = RsUtil::GetDimension().y;

    if( ApConfig::IsWin() )
	{
		static BtBool isReset = BtTrue;
		if( isReset )
		{
			isReset = BtFalse;

			// Render the mouse pointers
			for (BtU32 i = 0; i < MaxTouches; i++)
			{
				m_v2Position[i] = ShTouch::GetPosition(i);
				m_v2Position[i].x = m_v2Position[i].x / width  * v2Dimension.x;
				m_v2Position[i].y = m_v2Position[i].y / height * v2Dimension.y;
			}
		}

		// Render the mouse pointers
		for( BtU32 i = 0; i < MaxTouches; i++ )
		{
			if( ShTouch::IsHeld(i) )
			{
				m_isVisible[i] = BtTrue;
			}

			MtVector2 v2Position = ShTouch::GetPosition(i);
			v2Position.x = v2Position.x / width  * v2Dimension.x;
			v2Position.y = v2Position.y / height * v2Dimension.y;
			MtVector2 v2Delta = m_v2Position[i] - v2Position;

			if (v2Delta.GetLength() )
			{
				m_isVisible[i] = BtTrue;
			}
			
			if( m_isVisible[i] )
			{
				BtFloat alpha = 1.0f;

				if( ShTouch::IsHeld(i) == BtFalse )
				{
					alpha = 0.25f;
				}
				else
				{
					alpha = 1.0f;
				}

				const BtU32 sprite = 1;
				MtVector2 v2Dimension = m_pSprite->GetDimension( sprite );

				v2Position.x -= v2Dimension.x * ( 0.40f * m_scalar );
				v2Position.y -= v2Dimension.y * ( 0.15f * m_scalar );

				m_pSprite->Render( BtFalse,
					v2Position,
					v2Dimension * m_scalar,
					sprite,
					0,
					RsColour(1.0f, 1.0f, 1.0f, alpha),
					MaxSortOrders - 1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// HlJoysticks.cpp

// Includes
#include "BaArchive.h"
#include "HlJoysticks.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "RsUtil.h"
#include "RsShader.h"
#include "ShJoystick.h"
#include "HlDraw.h"
#include "SgNode.h"

//static
RsMaterial *HlJoysticks::m_pMaterial2 = BtNull;
RsMaterial *HlJoysticks::m_pMaterial3 = BtNull;
SgNode *HlJoysticks::m_pModel[2];
RsMaterial *HlJoysticks::m_pMaterial3NoZTest = BtNull;
BtBool HlJoysticks::m_ztest = BtFalse;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlJoysticks::Setup( SgNode *pNode1, SgNode *pNode2 )
{
	m_pModel[0] = pNode1;
	m_pModel[1] = pNode2;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlJoysticks::Setup( RsMaterial *pMaterial2,
						 RsMaterial *pMaterial3,
						 RsMaterial *pMaterial3NoZTest )
{
	m_pMaterial2 = pMaterial2;
	m_pMaterial3 = pMaterial3;
	m_pMaterial3NoZTest = pMaterial3NoZTest;
}

////////////////////////////////////////////////////////////////////////////////
// SetZTest

void HlJoysticks::SetZTest( BtBool isEnabled )
{
	m_ztest = isEnabled;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlJoysticks::Render()
{
	for (int i = 0; i < MaxJoysticks; i++)
	{
		if (ShJoystick::IsConnected(i))
		{
			MtMatrix4 m4Transform = ShJoystick::GetTransform(i);
			if (m_pModel[i])
			{
				m_pModel[i]->SetLocalTransform(m4Transform);
				m_pModel[i]->Update();
				m_pModel[i]->Render();
			}
			else
			{
				HlDraw::RenderCross(m4Transform, 0.1f, MaxSortOrders - 1);
			}

			BtFloat trigger = ShJoystick::GetTrigger(i);

			if (trigger)
			{
				MtVector3 v3From = m4Transform.GetTranslation();
				MtVector3 v3To = v3From + m4Transform.GetRotation().ZAxis() * trigger;

				HlDraw::RenderLine(v3From, v3To, RsColour::WhiteColour(), MaxSortOrders - 1);
			}
		}
	}
}

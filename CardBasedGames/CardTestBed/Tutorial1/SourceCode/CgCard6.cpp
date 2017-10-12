////////////////////////////////////////////////////////////////////////////////
// CgCard6.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard6.h"
#include "UiKeyboard.h"
#include "SbMain.h"
#include "MtMatrix4.h"
#include "SgNode.h"
#include "HlModel.h"
#include "BtTime.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard6::Setup()
{
	HlView::Setup("card6", "card6", BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard6::OnEntry()
{
	m_pNode = m_archive.GetNode("cube");
	HlModel::SetSortOrders(m_pNode, MaxSortOrders - 1);
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard6::OnExit()
{
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard6::Update()
{
	HlView::Update();

	static BtFloat z = 0.0f;
	MtMatrix4 m4Rotate;
	m4Rotate.SetRotationY(z);

	if (!ApConfig::IsPaused())
	{
		z -= BtTime::GetTick();
	}

	MtMatrix4 m4Transform;
	m4Transform.SetTranslation(MtVector3(0, 0, 6.0f));
	m4Transform = m4Rotate * m4Transform;
	m_pNode->SetLocalTransform(m4Transform);
	m_pNode->Update();

	// Cache each item
	HlMenuItems &item = m_items[0];

	if (item.m_isReleased)
	{
		SbMain::SetNextState(6);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard6::Render()
{
	HlView::Render();

	m_pNode->Render();
}

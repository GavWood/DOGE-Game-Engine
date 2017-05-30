////////////////////////////////////////////////////////////////////////////////
// CgCard4.cpp

// Includes
#include "BaArchive.h"
#include "MtVector2.h"
#include "RsUtil.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "SdSound.h"
#include "HlFont.h"
#include "HlScreenSize.h"
#include "CgCard.h"
#include "SbMain.h"
#include "RsUtil.h"
#include "HlDraw.h"
#include "UiKeyboard.h"
#include "PeerToPeer.h"
#include "BaArchive.h"
#include "RsSprite.h"
#include "ShTouch.h"
#include "corona.h"
#include "RsTexture.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void CgCard::Setup( BaArchive *archive )
{
	HlView::Setup("card", "card", BtTrue);
	m_pArchive = archive;
	m_pSprite = m_pArchive->GetSprite("MousePointers");
	m_v2Position = MtVector2(0, 0);
}

////////////////////////////////////////////////////////////////////////////////
// OnEntry

void CgCard::OnEntry()
{
	LoadCard(0);
}

////////////////////////////////////////////////////////////////////////////////
// OnExit

void CgCard::OnExit()
{
	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void CgCard::Update()
{
	HlView::Update();

	// Cache each item
	HlMenuItems &item = m_items[0];

	if (item.m_isReleased)
	{
		SbMain::SetNextState();
	}
}

////////////////////////////////////////////////////////////////////////////////
// LoadCard

void CgCard::LoadCard(BtU32 index)
{
	if (m_pBackgroundMaterial)
	{
		BtChar szFilename[256];
        sprintf(szFilename, "%s%scard%d.png", ApConfig::GetResourcePath(),
                ApConfig::GetDelimitter(), index + 1); // because cards start at ace=1
		corona::Image* pImage = corona::OpenImage(szFilename, corona::PF_B8G8R8A8);
        
        if( pImage == BtNull )
        {
            printf("%s", szFilename );
        }

		if (pImage != BtNull)
		{
			BtU8 *pMemory = (BtU8*)pImage->getPixels();

			// Cache the texture from the background material
			RsTexture *pTexture = m_pBackgroundMaterial->GetTexture(0);

			BtU32 textureSize = pTexture->GetOriginalWidth() * 4 *
				pTexture->GetOriginalHeight();

			pTexture->WriteMemory(pMemory, textureSize);

			// Delete the image
			delete pImage;

			int a = 0;
			a++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void CgCard::Render()
{
	BtChar text[256];
	HlView::Render();

	sprintf(text, "Debug card number %d", SbMain::GetState() );

	if (ApConfig::IsDebug())
	{
		MtVector2 v2Dimension = HlFont::GetDimension(text);
		MtVector2 v2Position = RsUtil::GetDimension() / 2.0f;
		v2Position.x = v2Position.x - (v2Dimension.x * 0.5f);
		v2Position.y = RsUtil::GetWidth() / 10.0f;
		HlFont::RenderHeavy(v2Position, text, MaxSortOrders - 1);

		RsColour colour(1.0f, 1.0f, 1.0f, 0.1f);
		m_pSprite->Render(BtFalse, m_v2Position, 1, 0, colour, MaxSortOrders - 1);
	}

	if (ApConfig::IsDebug())
	{
		sprintf(text, "First peer %d\nPing %d", MpPeerToPeer::IsFirstPeer(), m_pingCount);

		MtVector2 v2Position = RsUtil::GetDimension() / 10.0f;
		MtVector2 v2Dimension = HlFont::GetDimension(text);
		v2Position.x = v2Position.x - (v2Dimension.x * 0.5f);
		HlFont::RenderHeavy(v2Position, text, MaxSortOrders - 1);

		RsColour colour(1.0f, 1.0f, 1.0f, 0.1f);
		m_pSprite->Render(BtFalse, m_v2Position, 1, 0, colour, MaxSortOrders - 1);
	}
}

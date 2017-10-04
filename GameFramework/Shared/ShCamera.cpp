////////////////////////////////////////////////////////////////////////////////
// ShCamera.cpp

// Includes
#include "ShCamera.h"
#include "RsUtil.h"
#include "BaArchive.h"
#include "RsMaterial.h"
#include "RsTexture.h"

BaArchive *ShCamera::m_pArchive = BtNull;
RsMaterial *ShCamera::m_pMaterial;

////////////////////////////////////////////////////////////////////////////////
// Create

//static
void ShCamera::Create( BaArchive *pArchive )
{
    m_pArchive = pArchive;
    m_pMaterial = pArchive->GetMaterial( "yuv" );
    m_pMaterial->SetTechniqueName( "RsYUVToRGB" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShCamera::Update( BtU32 y, BtU32 crcb )
{
    BtAssert( m_pArchive );
    
    // Override the texture used on this material
    m_pMaterial->GetTexture(0)->SetTexture(y);
    m_pMaterial->GetTexture(1)->SetTexture(crcb);
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

//static
void ShCamera::Destroy()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

//static
void ShCamera::Render()
{
    // Allocate vertex
    RsVertex3 vertex[4];
    RsVertex3 *pVertex = &vertex[0];
    
    // Calculate the positions
    BtFloat xmax = RsUtil::GetWidth();
    BtFloat ymax = RsUtil::GetHeight();
    //xmax = 256.0f;
    //ymax = 256.0f;
    
    RsVertex3 *pQuad = pVertex;
    
    // Copy these into vertex
    pVertex->m_v3Position = MtVector3(0, 0, 0.1f );
    pVertex->m_v2UV = MtVector2( 1, 1 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(0, ymax, 0.1f );
    pVertex->m_v2UV = MtVector2( 1, 0 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(xmax, 0, 0.1f );
    pVertex->m_v2UV = MtVector2( 0, 1 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(xmax, ymax, 0.1f );
    pVertex->m_v2UV = MtVector2( 0, 0 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    // Cache the display width and height
    BtFloat fScaleWidth  = 1.0f / RsUtil::GetWidth();
    BtFloat fScaleHeight = 1.0f / RsUtil::GetHeight();
    
    // Scale the position to local screen space -1 to 1
    for( BtU32 i=0; i<4; i++ )
    {
        // Flip the y
        pQuad[ i ].m_v3Position.y = RsUtil::GetHeight() - pQuad[ i ].m_v3Position.y;
        
        // Scale from 0..width to 0..1
        pQuad[ i ].m_v3Position.x *= fScaleWidth;
        pQuad[ i ].m_v3Position.y *= fScaleHeight;
        
        // Scale from 0..1 to 0..2
        pQuad[ i ].m_v3Position.x *= 2.0f;
        pQuad[ i ].m_v3Position.y *= 2.0f;
        
        // Translate from 0..2 to -1..1
        pQuad[ i ].m_v3Position.x -= 1.0f;
        pQuad[ i ].m_v3Position.y -= 1.0f;
    }
    
    m_pMaterial->Render( RsPT_TriangleStrip, pQuad, 4, 0 );
}



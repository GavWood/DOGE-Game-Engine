////////////////////////////////////////////////////////////////////////////////
// HlMaterial.cpp

#include "HlMaterial.h"
#include "RsVertex.h"
#include "RsUtil.h"
#include "RsMaterial.h"
#include "ApConfig.h"
#include "HlMaterial.h"

////////////////////////////////////////////////////////////////////////////////
// RenderFullscreen

void HlMaterial::RenderFullscreen( RsMaterial *pMaterial, BtU32 sortorder )
{
    pMaterial->Render( MtVector2( 0, 0 ), RsRenderTarget::GetCurrent()->GetCameraDimension(), RsColour::WhiteColour(), sortorder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderUpscaledWithBorders

void HlMaterial::RenderUpscaledWithBorders(RsMaterial *pMaterial, BtU32 sortorder)
{
	// Get the original dimension of the background image
	MtVector2 v2OriginalDimension = pMaterial->GetDimension();

	// Get the current resolution
	MtVector2 v2CurrentResolution = RsRenderTarget::GetCurrent()->GetCameraDimension();

	BtFloat ratioX = v2CurrentResolution.x / v2OriginalDimension.x;
	BtFloat ratioY = v2CurrentResolution.y / v2OriginalDimension.y;

	if( (v2CurrentResolution.x < v2OriginalDimension.x) || 
		(v2CurrentResolution.y < v2OriginalDimension.y) )
	{
		ratioX = MtMin(ratioX, ratioY);
		ratioY = ratioX;
	}
	else
	{
		ratioX = MtMin(ratioX, ratioY);
		ratioY = ratioX;
	}

	// Construct the new dimension
	MtVector2 v2NewDimension(v2OriginalDimension.x * ratioX, v2OriginalDimension.y * ratioX);
	MtVector2 v2Position = RsUtil::GetHalfDimension() - (v2NewDimension / 2);
	pMaterial->Render(v2Position, v2NewDimension, RsColour::WhiteColour(), MaxSortOrders-1 );
}

////////////////////////////////////////////////////////////////////////////////
// RenderUpsideDown

void HlMaterial::RenderUpsideDownXFlip( RsMaterial *pMaterial,
                                        const MtVector2 &v2Position,
                                        const MtVector2 &v2Dimension,
                                        const MtVector2 &v2ScreenDimension,
                                        BtU32 sortOrder
                                  )
{
    RsVertex3 vertex[4];
    RsVertex3 *pVertex = &vertex[0];
    
    // Calculate the positions
    BtFloat xmax = v2Dimension.x;
    BtFloat ymax = v2Dimension.y;
    
    RsVertex3 *pQuad = pVertex;
    
    // Copy these into vertex
    pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y, 0.1f);
    pVertex->m_v2UV = MtVector2(1, 0);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y + ymax, 0.1f);
    pVertex->m_v2UV = MtVector2(1, 1);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + 0, 0.1f);
    pVertex->m_v2UV = MtVector2(0, 0);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + ymax, 0.1f);
    pVertex->m_v2UV = MtVector2(0, 1);
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    // Flip the V's
    pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
    pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
    pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
    pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;
    
    // Cache the display width and height
    MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
    BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
    BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;
    
    // Scale the position to local screen space -1 to 1
    for(BtU32 i = 0; i < 4; i++)
    {
        // Flip the y
        pQuad[i].m_v3Position.y = v2TargetDimension.y - pQuad[i].m_v3Position.y;
        
        // Scale from 0..width to 0..1
        pQuad[i].m_v3Position.x *= fScaleWidth;
        pQuad[i].m_v3Position.y *= fScaleHeight;
        
        // Scale from 0..1 to 0..2
        pQuad[i].m_v3Position.x *= 2.0f;
        pQuad[i].m_v3Position.y *= 2.0f;
        
        // Translate from 0..2 to -1..1
        pQuad[i].m_v3Position.x -= 1.0f;
        pQuad[i].m_v3Position.y -= 1.0f;
    }
    
    pMaterial->Render(RsPT_TriangleStrip, pQuad, 4, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderUpsideDown

void HlMaterial::RenderUpsideDown( RsMaterial *pMaterial,
								   const MtVector2 &v2Position,
								   const MtVector2 &v2Dimension,
								   const MtVector2 &v2ScreenDimension,
                                   BtU32 sortOrder
								  )
{
	RsVertex3 vertex[4];
	RsVertex3 *pVertex = &vertex[0];

	// Calculate the positions
	BtFloat xmax = v2Dimension.x;
	BtFloat ymax = v2Dimension.y;

	RsVertex3 *pQuad = pVertex;

	// Copy these into vertex
	pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y, 0.1f);
	pVertex->m_v2UV = MtVector2(0, 0);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x, v2Position.y + ymax, 0.1f);
	pVertex->m_v2UV = MtVector2(0, 1);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + 0, 0.1f);
	pVertex->m_v2UV = MtVector2(1, 0);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(v2Position.x + xmax, v2Position.y + ymax, 0.1f);
	pVertex->m_v2UV = MtVector2(1, 1);
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	// Flip the V's
	pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
	pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
	pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
	pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;

	// Cache the display width and height
	MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
	BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
	BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;

	// Scale the position to local screen space -1 to 1
	for(BtU32 i = 0; i < 4; i++)
	{
		// Flip the y
		pQuad[i].m_v3Position.y = v2TargetDimension.y - pQuad[i].m_v3Position.y;

		// Scale from 0..width to 0..1
		pQuad[i].m_v3Position.x *= fScaleWidth;
		pQuad[i].m_v3Position.y *= fScaleHeight;

		// Scale from 0..1 to 0..2
		pQuad[i].m_v3Position.x *= 2.0f;
		pQuad[i].m_v3Position.y *= 2.0f;

		// Translate from 0..2 to -1..1
		pQuad[i].m_v3Position.x -= 1.0f;
		pQuad[i].m_v3Position.y -= 1.0f;
	}

	pMaterial->Render(RsPT_TriangleStrip, pQuad, 4, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderSideWays

void HlMaterial::RenderSideWays( RsMaterial *pMaterial,
                                 const MtVector2 &v2Position,
                                 const MtVector2 &v2Dimension
                                )
{
    // Allocate vertex
    RsVertex3 vertex[4];
    RsVertex3 *pVertex = &vertex[0];
    
    // Calculate the positions
    BtFloat xmax = v2Dimension.x;
    BtFloat ymax = v2Dimension.y;
    
    RsVertex3 *pQuad = pVertex;
    
    // Copy these into vertex
    pVertex->m_v3Position = MtVector3( v2Position.x, v2Position.y, 0.1f );
    pVertex->m_v2UV = MtVector2( 0, 0 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3( v2Position.x, v2Position.y + ymax, 0.1f );
    pVertex->m_v2UV = MtVector2( 1, 0 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3( v2Position.x + xmax, v2Position.y + 0, 0.1f );
    pVertex->m_v2UV = MtVector2( 0, 1 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    pVertex->m_v3Position = MtVector3( v2Position.x + xmax, v2Position.y + ymax, 0.1f );
    pVertex->m_v2UV = MtVector2( 1, 1 );
    pVertex->m_colour = 0xFFFFFFFF;
    ++pVertex;
    
    // Flip the V's
    pQuad[0].m_v2UV.y = 1.0f - pQuad[0].m_v2UV.y;
    pQuad[1].m_v2UV.y = 1.0f - pQuad[1].m_v2UV.y;
    pQuad[2].m_v2UV.y = 1.0f - pQuad[2].m_v2UV.y;
    pQuad[3].m_v2UV.y = 1.0f - pQuad[3].m_v2UV.y;
    
    // Cache the display width and height
	MtVector2 v2TargetDimension = RsRenderTarget::GetCurrent()->GetCameraDimension();
	BtFloat fScaleWidth = 1.0f / v2TargetDimension.x;
	BtFloat fScaleHeight = 1.0f / v2TargetDimension.y;
    
    // Scale the position to local screen space -1 to 1
    for( BtU32 i=0; i<4; i++ )
    {
        // Flip the y
        pQuad[ i ].m_v3Position.y = v2TargetDimension.y - pQuad[ i ].m_v3Position.y;
        
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
    
    pMaterial->Render( RsPT_TriangleStrip, pQuad, 4, MaxSortOrders-1 );
}

////////////////////////////////////////////////////////////////////////////////
// RenderRightWayUp

void HlMaterial::RenderRightWayUp( RsMaterial *pMaterial,
								   const MtVector2 &v3Position,
								   const MtVector2 &v2Dimension,
								   const MtVector2 &v2ScreenDimension
								  )
{
	// Allocate vertex
	RsVertex3 vertex[4];
	RsVertex3 *pVertex = &vertex[0];

	// Calculate the positions
	BtFloat xmax = v2Dimension.x;
	BtFloat ymax = v2Dimension.y;

	RsVertex3 *pQuad = pVertex;

	// Copy these into vertex
	pVertex->m_v3Position = MtVector3(0, 0, 0.1f );
	pVertex->m_v2UV = MtVector2( 0, 0 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(0, ymax, 0.1f );
	pVertex->m_v2UV = MtVector2( 0, 1 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmax, 0, 0.1f );
	pVertex->m_v2UV = MtVector2( 1, 0 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmax, ymax, 0.1f );
	pVertex->m_v2UV = MtVector2( 1, 1 );
	pVertex->m_colour = 0xFFFFFFFF;
	++pVertex;

	// Cache the display width and height
	BtFloat fScaleWidth  = 1.0f / RsUtil::GetWidth();
	BtFloat fScaleHeight = 1.0f / RsUtil::GetHeight();

	// Scale the position to local screen space -1 to 1
	for( BtU32 i=0; i<4; i++ )
	{
		// Set the colour
		pQuad[ i ].m_v3Position.x -= 0.5f;
		pQuad[ i ].m_v3Position.y -= 0.5f;

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

	pMaterial->Render( RsPT_TriangleStrip, pQuad, 4, MaxSortOrders-1 );
}

////////////////////////////////////////////////////////////////////////////////
// SetRenderTarget

void HlMaterial::SetRenderTarget( RsMaterial *pBackBuffer )
{
	MtVector2 v2Dimension = pBackBuffer->GetDimension();
	RsCamera camera = RsCamera(v2Dimension.x, v2Dimension.y, 0.1f, 10000.0f, RsViewport(0, 0, (BtU32)v2Dimension.x, (BtU32)v2Dimension.y));
	camera.SetDimension(v2Dimension);
	camera.SetPerspective(BtFalse);
	camera.Update();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set a default camera for all the render targets
	pRenderTarget->SetCamera(camera);

	// Clear the render target
	pRenderTarget->SetCleared(BtTrue);

	// Clear the z buffer
	pRenderTarget->SetZCleared(BtTrue);

	// Set the colour
	pRenderTarget->SetClearColour( RsColour::NoColour() );

	// Set the render target
	pRenderTarget->SetTexture( pBackBuffer->GetTexture(0) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// RenderBillboard

void HlMaterial::RenderBillboard(RsCamera camera, RsMaterial *pBackBuffer)
{
	BtFloat completeFieldOfView = camera.GetFieldOfView();
	MtMatrix3 m3Orientation = camera.GetRotation().GetInverse();
	
	MtVector3 v3AxisXScaled = m3Orientation.XAxis() * 0.5f;
	MtVector3 v3AxisYScaled = m3Orientation.YAxis() * 0.5f;
	MtVector3 v3AxisZ = m3Orientation.ZAxis();

	BtFloat fieldOfView = completeFieldOfView * 0.5f;
	BtFloat tanAngle = MtTan(fieldOfView);
	BtFloat distance = 0.5f / tanAngle;

	MtVector3 v3Position = camera.GetPosition() + (v3AxisZ * distance);

	RsVertex3 m_v3Vertex[6];

	//
	m_v3Vertex[0].m_v3Position.x = v3Position.x - v3AxisXScaled.x - v3AxisYScaled.x;
	m_v3Vertex[0].m_v3Position.y = v3Position.y - v3AxisXScaled.y - v3AxisYScaled.y;
	m_v3Vertex[0].m_v3Position.z = v3Position.z - v3AxisXScaled.z - v3AxisYScaled.z;
	m_v3Vertex[0].m_v2UV.x = 0;
	m_v3Vertex[0].m_v2UV.y = 0;

	m_v3Vertex[1].m_v3Position.x = v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
	m_v3Vertex[1].m_v3Position.y = v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
	m_v3Vertex[1].m_v3Position.z = v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
	m_v3Vertex[1].m_v2UV.x = 1;
	m_v3Vertex[1].m_v2UV.y = 0;

	m_v3Vertex[2].m_v3Position.x = v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
	m_v3Vertex[2].m_v3Position.y = v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
	m_v3Vertex[2].m_v3Position.z = v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
	m_v3Vertex[2].m_v2UV.x = 0;
	m_v3Vertex[2].m_v2UV.y = 1;

	// Second triangle
	m_v3Vertex[3].m_v3Position.x = v3Position.x - v3AxisXScaled.x + v3AxisYScaled.x;
	m_v3Vertex[3].m_v3Position.y = v3Position.y - v3AxisXScaled.y + v3AxisYScaled.y;
	m_v3Vertex[3].m_v3Position.z = v3Position.z - v3AxisXScaled.z + v3AxisYScaled.z;
	m_v3Vertex[3].m_v2UV.x = 0;
	m_v3Vertex[3].m_v2UV.y = 1;

	m_v3Vertex[4].m_v3Position.x = v3Position.x + v3AxisXScaled.x - v3AxisYScaled.x;
	m_v3Vertex[4].m_v3Position.y = v3Position.y + v3AxisXScaled.y - v3AxisYScaled.y;
	m_v3Vertex[4].m_v3Position.z = v3Position.z + v3AxisXScaled.z - v3AxisYScaled.z;
	m_v3Vertex[4].m_v2UV.x = 1;
	m_v3Vertex[4].m_v2UV.y = 0;

	m_v3Vertex[5].m_v3Position.x = v3Position.x + v3AxisXScaled.x + v3AxisYScaled.x;
	m_v3Vertex[5].m_v3Position.y = v3Position.y + v3AxisXScaled.y + v3AxisYScaled.y;
	m_v3Vertex[5].m_v3Position.z = v3Position.z + v3AxisXScaled.z + v3AxisYScaled.z;
	m_v3Vertex[5].m_v2UV.x = 1;
	m_v3Vertex[5].m_v2UV.y = 1;

	for (BtU32 i = 0; i < 6; i++)
	{
		m_v3Vertex[i].m_colour = RsColour::WhiteColour().asWord();
		m_v3Vertex[i].m_v3Normal = MtVector3(0, 1, 0);

		if (ApConfig::GetPlatform() == ApPlatform_DX11)
		{
			m_v3Vertex[i].m_v2UV.y = 1.0f - m_v3Vertex[i].m_v2UV.y;
		}
	}
	pBackBuffer->Render(RsPT_TriangleList, m_v3Vertex, 6, MaxSortOrders - 1, BtTrue);
}

////////////////////////////////////////////////////////////////////////////////
// RenderSidewaysUpscaledWithBorders

void HlMaterial::RenderSidewaysUpscaledWithBorders(RsMaterial *pMaterial, BtU32 sortorder)
{
    // Get the original dimension of the background image
    MtVector2 v2OriginalDimension = pMaterial->GetDimension();
    
    // Swap the dimensions to calculate the ratio of y / x, rather than x / y
    BtSwap( v2OriginalDimension.x, v2OriginalDimension.y );

    // Get the current resolution
    MtVector2 v2CurrentResolution = RsRenderTarget::GetCurrent()->GetCameraDimension();
    
    BtFloat ratioX = v2CurrentResolution.x / v2OriginalDimension.x;
    BtFloat ratioY = v2CurrentResolution.y / v2OriginalDimension.y;
    
    if( (v2CurrentResolution.x < v2OriginalDimension.x) ||
        (v2CurrentResolution.y < v2OriginalDimension.y) )
    {
        ratioX = MtMin(ratioX, ratioY);
        ratioY = ratioX;
    }
    else
    {
        ratioX = MtMin(ratioX, ratioY);
        ratioY = ratioX;
    }
    
    // Construct the new dimension
    MtVector2 v2NewDimension(v2OriginalDimension.x * ratioX, v2OriginalDimension.y * ratioX);
    MtVector2 v2Position = RsUtil::GetHalfDimension() - (v2NewDimension / 2);
    
    // Render it sideways
    RenderSideWays( pMaterial, v2Position, v2NewDimension );
}

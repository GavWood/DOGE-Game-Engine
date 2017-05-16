////////////////////////////////////////////////////////////////////////////////
// HlDraw.cpp

// Includes
#include "BaArchive.h"
#include "HlDraw.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "RsUtil.h"
#include "RsShader.h"

//static
RsMaterial *HlDraw::m_pMaterial2 = BtNull;
RsMaterial *HlDraw::m_pMaterial3 = BtNull;
RsMaterial *HlDraw::m_pMaterial3NoZTest = BtNull;
BtBool HlDraw::m_ztest = BtFalse;
RsColour HlDraw::m_colour1;
RsColour HlDraw::m_colour2;
RsColour HlDraw::m_colour3;
BtFloat HlDraw::m_arrowThickness = 0.05f;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlDraw::Setup( RsMaterial *pMaterial2,
					RsMaterial *pMaterial3,
					RsMaterial *pMaterial3NoZTest )
{
	m_pMaterial2 = pMaterial2;
	m_pMaterial3 = pMaterial3;
	m_pMaterial3NoZTest = pMaterial3NoZTest;
	m_colour1 = RsColour::GreenColour();
	m_colour2 = RsColour::WhiteColour();
	m_colour3 = RsColour::BlueColour();
}

////////////////////////////////////////////////////////////////////////////////
// SetZTest

void HlDraw::SetZTest( BtBool isEnabled )
{
	m_ztest = isEnabled;
}

////////////////////////////////////////////////////////////////////////////////
// RenderLine

void HlDraw::RenderLine( const MtVector3 &v3Start, const MtVector3 &v3End, const RsColour &colour, BtU32 sortOrder )
{
	// Allocate vertex
	RsVertex3 vertex[2];

	BtU32 col = colour.asWord();

	// Copy these into vertex
	vertex[0].m_v3Position = v3Start;
	vertex[0].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[0].m_v3Normal = MtVector3( 0, 0, 1 );
	vertex[0].m_colour = col;

	vertex[1].m_v3Position = v3End;
	vertex[1].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[1].m_v3Normal = MtVector3( 0, 0, 1 );
	vertex[1].m_colour = col;

	if (m_ztest == BtTrue)
	{
		m_pMaterial3->Render(RsPT_LineList, vertex, 2, sortOrder);
	}
	else
	{
		m_pMaterial3NoZTest->Render(RsPT_LineList, vertex, 2, sortOrder);
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderArrow

void HlDraw::RenderArrow(const MtVector3 &v3A, const MtVector3 &v3B, MtVector3 v3Axis, RsColour colour, BtU32 sortOrder)
{
	BtU32 col = colour.asWord();

	MtVector3 v3Delta = v3B - v3A;
	MtVector3 v3C = v3B - (v3Delta * 0.3f);

	BtFloat width = v3Delta.GetLength() * m_arrowThickness;

	MtVector3 v3VerticalArrow = v3Axis * width;

	RsVertex3 vertex[4];
	vertex[1].m_v3Position = v3A - v3VerticalArrow;
	vertex[1].m_v2UV.x = 0;
	vertex[1].m_v2UV.y = 0;
	vertex[1].m_colour = col;

	vertex[0] = vertex[1];
	vertex[0].m_v3Position = v3A + v3VerticalArrow;

	vertex[3].m_v3Position = v3C - v3VerticalArrow;
	vertex[3].m_v2UV.x = 0;
	vertex[3].m_v2UV.y = 0;
	vertex[3].m_colour = col;

	vertex[2] = vertex[3];
	vertex[2].m_v3Position = v3C + v3VerticalArrow;

	if(m_ztest == BtTrue)
	{
		m_pMaterial3->Render(RsPT_TriangleStrip, vertex, 4, sortOrder);
	}
	else
	{
		m_pMaterial3NoZTest->Render(RsPT_TriangleStrip, vertex, 4, sortOrder);
	}

	MtVector3 v3VerticalArrowHead = v3Axis * width * 2.0f;

	vertex[1].m_v3Position = v3C - v3VerticalArrowHead;
	vertex[1].m_v2UV.x = 0;
	vertex[1].m_v2UV.y = 0;
	vertex[1].m_colour = col;

	vertex[0].m_v3Position = v3C + v3VerticalArrowHead;
	vertex[0].m_v2UV.x = 0;
	vertex[0].m_v2UV.y = 0;
	vertex[0].m_colour = col;

	vertex[3].m_v3Position.x = v3B.x;
	vertex[3].m_v3Position.y = v3B.y;
	vertex[3].m_v3Position.z = v3B.z;
	vertex[3].m_v2UV.x = 0;
	vertex[3].m_v2UV.y = 0;
	vertex[3].m_colour = col;

	vertex[2] = vertex[3];

	if(m_ztest == BtTrue)
	{
		m_pMaterial3->Render(RsPT_TriangleStrip, vertex, 4, sortOrder);
	}
	else
	{
		m_pMaterial3NoZTest->Render(RsPT_TriangleStrip, vertex, 4, sortOrder);
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderCG

void HlDraw::RenderCG(const MtVector3 &v3Position, BtFloat width, BtU32 sortOrder)
{
	MtMatrix3 m3Rotation = RsRenderTarget::GetCurrent()->GetCamera().GetRotation().GetInverse();

    const BtU32 NumSegments = 32;
	RsVertex3 vertex[32 * 3 + 3];
	vertex[0].m_colour = RsColour::WhiteColour().asWord();
	vertex[0].m_v2UV = vertex[0].m_v2UV2 = MtVector2(0.5f, 0.5f);
	vertex[0].m_v3Normal = -m3Rotation.Row2();
	vertex[0].m_v3Position = v3Position;

	BtFloat circleInc = 360.0f / NumSegments;
	BtFloat current = 0;
	BtU32 index = 0;
	MtVector3 v3AxisX, v3AxisY;

	for(BtU32 i = 0; i < NumSegments; i++)
	{
		vertex[index + 0] = vertex[0];
		vertex[index + 1] = vertex[0];
		vertex[index + 2] = vertex[0];

		// 0
		vertex[index + 0].m_v3Position = v3Position;

		// 1
		v3AxisX = m3Rotation.Row0() * MtCos(MtDegreesToRadians(current));
		v3AxisY = m3Rotation.Row1() * MtSin(MtDegreesToRadians(current));
		vertex[index + 1].m_v3Position = v3Position + (v3AxisX * width) + (v3AxisY * width);
		current += circleInc;

		// 2
		v3AxisX = m3Rotation.Row0() * MtCos(MtDegreesToRadians(current));
		v3AxisY = m3Rotation.Row1() * MtSin(MtDegreesToRadians(current));
		vertex[index + 2].m_v3Position = v3Position + (v3AxisX * width) + (v3AxisY * width);

		index += 3;
	}

	// Alternate the colours
	BtU32 count = index / 4;
	BtU32 vert = 0;
	RsColour colour = RsColour::WhiteColour();
	for( BtU32 i=0; i<4; i++ )
	{
		if( i % 2 )
		{
			colour = RsColour::BlackColour();
		}
		else
		{
			colour = RsColour::WhiteColour();
		}

		for( BtU32 j=0; j<count; j++ )
		{
			vertex[vert++].m_colour = colour.asWord();
		}
	}
	m_pMaterial3NoZTest->Render(RsPT_TriangleList, vertex, index, sortOrder);
}

////////////////////////////////////////////////////////////////////////////////
// RenderTriangleStrip

void HlDraw::RenderTriangleStrip(const MtVector3 *pv3Position, BtU32 numVertex, const RsColour &colour, BtU32 sortOrder)
{
	RsVertex3 vertex[32 * 3 + 3];
	vertex[0].m_colour = colour.asWord();
	vertex[0].m_v2UV = vertex[0].m_v2UV2 = MtVector2(0.5f, 0.5f);
	vertex[0].m_v3Normal = MtVector3(0, 1, 0);
	vertex[0].m_v3Position = pv3Position[0];

	for (BtU32 i = 0; i < numVertex; i++)
	{
		vertex[i + 0] = vertex[0];
		vertex[i].m_v3Position = pv3Position[i];
	}

	if (m_ztest == BtTrue)
	{
		m_pMaterial3->Render(RsPT_TriangleStrip, vertex, numVertex, sortOrder);
	}
	else
	{
		m_pMaterial3NoZTest->Render(RsPT_TriangleStrip, vertex, numVertex, sortOrder);
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderCircle

void HlDraw::RenderCircle( const MtVector3 &v3Position, const RsColour &colour, BtFloat width, BtU32 sortOrder )
{
	MtMatrix3 m3Rotation = RsRenderTarget::GetCurrent()->GetCamera().GetRotation().GetInverse();
	
	const BtU32 NumSegments = 32;
	RsVertex3 vertex[32 * 3 + 3];
	vertex[0].m_colour = RsColour::WhiteColour().asWord();
	vertex[0].m_v2UV = vertex[0].m_v2UV2 = MtVector2( 0.5f, 0.5f );
	vertex[0].m_v3Normal = -m3Rotation.Row2();
	vertex[0].m_v3Position = v3Position;

	BtFloat circleInc = 360.0f / NumSegments;
	BtFloat current = 0;
	BtU32 index = 0;
	MtVector3 v3AxisX, v3AxisY;

	for( BtU32 i=0; i<NumSegments; i++ )
	{
		vertex[index + 0] = vertex[0];
		vertex[index + 1] = vertex[0];
		vertex[index + 2] = vertex[0];

		// 0
		vertex[index + 0].m_v3Position = v3Position;

		// 1
		v3AxisX = m3Rotation.Row0() * MtCos( MtDegreesToRadians( current ) );
		v3AxisY = m3Rotation.Row1() * MtSin( MtDegreesToRadians( current ) );
		vertex[index + 1].m_v3Position = v3Position + (v3AxisX * width) + (v3AxisY * width);
		current += circleInc;

		// 2
		v3AxisX = m3Rotation.Row0() * MtCos( MtDegreesToRadians( current ));
		v3AxisY = m3Rotation.Row1() * MtSin( MtDegreesToRadians( current ));
		vertex[index + 2].m_v3Position = v3Position + (v3AxisX * width) + (v3AxisY * width);

		index += 3;
	}

	if(m_ztest == BtTrue)
	{
		m_pMaterial3->Render(RsPT_TriangleList, vertex, index, sortOrder);
	}
	else
	{
		m_pMaterial3NoZTest->Render(RsPT_TriangleList, vertex, index, sortOrder);
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderArrow

void HlDraw::RenderArrow( const MtVector3 &v3A, const MtVector3 &v3B, RsColour colour, BtU32 sortOrder)
{
	MtMatrix3 m3Rotation = RsRenderTarget::GetCurrent()->GetCamera().GetRotation().GetInverse();
	//MtVector3 v3Delta = v3A - v3B;
	RenderArrow( v3A, v3B, m3Rotation.YAxis(), colour, MaxSortOrders-1 );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlDraw::RenderLine( const MtVector2 &v2First, const MtVector2 &v2Second, const RsColour &colour, BtU32 sortOrder)
{
	BtAssert( RsShader::GetCurrent() );

	// Allocate vertex
	RsVertex2 vertex[2];

	BtU32 col = colour.asWord();

	// Copy these into vertex
	vertex[0].m_v2Position = MtVector2( v2First.x, v2First.y );
	vertex[0].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[0].m_colour = col;

	vertex[1].m_v2Position = MtVector2( v2Second.x, v2Second.y );
	vertex[1].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[1].m_colour = col;

	m_pMaterial2->Render( RsPT_LineList, vertex, 2, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// SetCrossColour

void HlDraw::SetCrossColour( const RsColour &colour1, const RsColour &colour2, const RsColour &colour3 )
{
	m_colour1 = colour1;
	m_colour2 = colour2;
	m_colour3 = colour3;
}

////////////////////////////////////////////////////////////////////////////////
// RenderCross

void HlDraw::RenderCross( const MtVector2 &v2Position, BtFloat radius, BtU32 sortOrder )
{
	MtVector2 v2A = v2Position - MtVector2(0, radius);
	MtVector2 v2B = v2Position + MtVector2(0, radius);
	HlDraw::RenderLine(v2A, v2B, m_colour1, sortOrder );

	v2A = v2Position - MtVector2(radius, 0);
	v2B = v2Position + MtVector2(radius, 0);
	HlDraw::RenderLine( v2A, v2B, m_colour2, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderArrow

void HlDraw::RenderArrow( const MtVector2 &v2A, const MtVector2 &v2B, BtFloat radius, BtU32 sortOrder )
{
	HlDraw::RenderLine( v2A, v2B, RsColour::BlackColour(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderCross

void HlDraw::RenderCross( const MtMatrix4 &m4Transforn, BtFloat length, BtU32 sortOrder )
{
	MtMatrix3 m3Rotation = RsRenderTarget::GetCurrent()->GetCamera().GetRotation().GetInverse();
	MtVector3 v3AxisX = m3Rotation.XAxis();
	MtVector3 v3AxisY = m3Rotation.YAxis();
	MtVector3 v3AxisZ = m3Rotation.ZAxis();
    (void)v3AxisZ;
    
	MtVector3 v3Position = m4Transforn.GetTranslation();

	MtVector3 v3HalfAxisX = m4Transforn.XAxis() * 0.5f * length;
	MtVector3 v3HalfAxisY = m4Transforn.YAxis() * 0.5f * length;
	MtVector3 v3HalfAxisZ = m4Transforn.ZAxis() * 0.5f * length;

	RenderArrow( v3Position - v3HalfAxisX, v3Position + v3HalfAxisX, v3AxisY, m_colour1, MaxSortOrders - 1);
	RenderArrow( v3Position - v3HalfAxisY, v3Position + v3HalfAxisY, v3AxisX, m_colour2, MaxSortOrders - 1);
	RenderArrow( v3Position - v3HalfAxisZ, v3Position + v3HalfAxisZ, v3AxisY, m_colour3, MaxSortOrders - 1);
}

////////////////////////////////////////////////////////////////////////////////
// RenderQuad

void HlDraw::RenderQuad( const MtVector2 &v2Position, MtVector2 v2Dimension, RsColour colour, BtU32 sortOrder )
{
	m_pMaterial2->Render( v2Position, v2Dimension, colour, sortOrder );
}

// RenderRotatedQuad

void HlDraw::RenderRotatedQuad(const MtVector2 &v2Position, MtVector2 v2Dimension, BtFloat angle, RsColour colour, BtU32 sortOrder)
{
	// Setup the dimension
	MtVector3 v3HalfDimension(v2Dimension.x * 0.5f, v2Dimension.y * 0.5f, 0);
	MtVector3 v3RotatedDimension[4];
	RsVertex2 vertex[4];

	MtVector3 v3Position = MtVector3(v2Position, 0);

	v3RotatedDimension[0] = MtVector3( -v3HalfDimension.x, -v3HalfDimension.y, 0.1f );
	v3RotatedDimension[1] = MtVector3( -v3HalfDimension.x,  v3HalfDimension.y, 0.1f );
	v3RotatedDimension[2] = MtVector3(  v3HalfDimension.x, -v3HalfDimension.y, 0.1f );
	v3RotatedDimension[3] = MtVector3(  v3HalfDimension.x,  v3HalfDimension.y, 0.1f );

	if (angle != 0)
	{
		MtMatrix4 m4Rotation;
		m4Rotation.SetRotationZ( MtDegreesToRadians( -angle ) );

		for (BtU32 i = 0; i < 4; i++)
		{
			v3RotatedDimension[i] *= m4Rotation;
			v3RotatedDimension[i] += v3Position;
		}
	}
	else
	{
		for (BtU32 i = 0; i < 4; i++)
		{
			v3RotatedDimension[i] += v3Position + v3HalfDimension;
		}
	}
	for (BtU32 i = 0; i < 4; i++)
	{
		vertex[i].m_v2Position = MtVector2(v3RotatedDimension[i].x, v3RotatedDimension[i].y);
		vertex[i].m_colour = colour.asWord();
	}

 	m_pMaterial2->Render(RsPT_TriangleStrip, vertex, 4, sortOrder);
}

////////////////////////////////////////////////////////////////////////////////
// RenderAABB

void HlDraw::RenderAABB( const MtAABB &aabb, MtMatrix4 m4Transform, RsColour colour, BtU32 sortOrder )
{
	MtVector3 v3Pos[8];
	v3Pos[0] = MtVector3( aabb.Min().x, aabb.Min().y, aabb.Min().z ); // 0 000
	v3Pos[1] = MtVector3( aabb.Min().x, aabb.Min().y, aabb.Max().z ); // 1 001
	v3Pos[2] = MtVector3( aabb.Min().x, aabb.Max().y, aabb.Min().z ); // 2 010
	v3Pos[3] = MtVector3( aabb.Min().x, aabb.Max().y, aabb.Max().z ); // 3 011
	v3Pos[4] = MtVector3( aabb.Max().x, aabb.Min().y, aabb.Min().z ); // 4 100
	v3Pos[5] = MtVector3( aabb.Max().x, aabb.Min().y, aabb.Max().z ); // 5 101
	v3Pos[6] = MtVector3( aabb.Max().x, aabb.Max().y, aabb.Min().z ); // 6 110
	v3Pos[7] = MtVector3( aabb.Max().x, aabb.Max().y, aabb.Max().z ); // 7 111
	for( BtU32 i=0; i<8; i++ )
	{
		v3Pos[i] *= m4Transform;
	}

	RenderLine( v3Pos[6], v3Pos[2], colour, sortOrder);
	RenderLine( v3Pos[4], v3Pos[0], colour, sortOrder);
	RenderLine( v3Pos[0], v3Pos[1], colour, sortOrder);
	RenderLine( v3Pos[7], v3Pos[3], colour, sortOrder );
	RenderLine( v3Pos[5], v3Pos[1], colour, sortOrder);
	RenderLine( v3Pos[5], v3Pos[7], colour, sortOrder);
	RenderLine( v3Pos[1], v3Pos[3], colour, sortOrder);
	RenderLine( v3Pos[2], v3Pos[3], colour, sortOrder);
	RenderLine( v3Pos[4], v3Pos[5], colour, sortOrder);
	RenderLine( v3Pos[6], v3Pos[7], colour, sortOrder);
	RenderLine( v3Pos[0], v3Pos[2], colour, sortOrder);
	RenderLine( v3Pos[4], v3Pos[6], colour, sortOrder);
}

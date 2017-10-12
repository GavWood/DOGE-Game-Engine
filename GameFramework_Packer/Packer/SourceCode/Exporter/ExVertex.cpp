////////////////////////////////////////////////////////////////////////////////
// ExVertex.cpp

// Includes
#include "BtString.h"
#include "ExVertex.h"
#include "PaTopState.h"
#include "ErrorLog.h"
#include "BtMemory.h"

BtChar s_Description[256];

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExVertex::ExVertex()
{
	BtMemory::Set( this, 0, sizeof(ExVertex) );
	m_vertexType = 0;
};

////////////////////////////////////////////////////////////////////////////////
// Description

BtChar* ExVertex::Description() const
{
	BtStrCopy( s_Description, 256, "" );

	if( m_vertexType & VT_Position )
	{
		BtStrCat( s_Description, 256, "Po" );
	}
	if( m_vertexType & VT_Normal )
	{
		BtStrCat( s_Description, 256, "No" );
	}
	if( m_vertexType & VT_Colour )
	{
		BtStrCat( s_Description, 256, "Co" );
	}
	if( m_vertexType & VT_UV0 )
	{
		BtStrCat( s_Description, 256, "U0" );
	}
	if( m_vertexType & VT_UV1 )
	{
		BtStrCat( s_Description, 256, "U1" );
	}
	if (m_vertexType & VT_Tangent)
	{
 		BtStrCat(s_Description, 256, "Ta");
	}
	if (m_vertexType & VT_Binormal)
	{
		BtStrCat(s_Description, 256, "Bn");
	}
	if( m_vertexType & VT_Skin )
	{
		BtStrCat( s_Description, 256, "Sk" );
	}
	if( m_vertexType & VT_BlendShape )
	{
		BtStrCat( s_Description, 256, "Bl" );
	}
	return s_Description;
}

////////////////////////////////////////////////////////////////////////////////
// Position

void ExVertex::Position( BtFloat x, BtFloat y, BtFloat z )
{
	m_v3Position = MtVector3( x, y, z );

	m_vertexType |= VT_Position;
}

////////////////////////////////////////////////////////////////////////////////
// Position

void ExVertex::Position( const MtVector3& v3Position )
{
	m_v3Position = v3Position;

	m_vertexType |= VT_Position;
}

////////////////////////////////////////////////////////////////////////////////
// Position1

void ExVertex::Position1( const MtVector3& v3Position )
{
	m_v3Position1 = v3Position;

	m_vertexType |= VT_BlendShape;
}

////////////////////////////////////////////////////////////////////////////////
// Position2

void ExVertex::Position2( const MtVector3& v3Position )
{
	m_v3Position2 = v3Position;

	m_vertexType |= VT_BlendShape;
}

////////////////////////////////////////////////////////////////////////////////
// Position3

void ExVertex::Position3( const MtVector3& v3Position )
{
	m_v3Position3 = v3Position;

	m_vertexType |= VT_BlendShape;
}

////////////////////////////////////////////////////////////////////////////////
// Normal

void ExVertex::Normal( BtFloat x, BtFloat y, BtFloat z )
{
	m_v3Normal = MtVector3( x, y, z );

	m_vertexType |= VT_Normal;
}

////////////////////////////////////////////////////////////////////////////////
// Tangent

void ExVertex::Tangent( BtFloat x, BtFloat y, BtFloat z )
{
	m_v3Tangent = MtVector3( x, y, z );

	m_vertexType |= VT_Tangent;
}

////////////////////////////////////////////////////////////////////////////////
// Binormal

void ExVertex::Binormal( BtFloat x, BtFloat y, BtFloat z )
{
	m_v3Bionormal = MtVector3( x, y, z );

	m_vertexType |= VT_Binormal;
}

////////////////////////////////////////////////////////////////////////////////
// UV

void ExVertex::UV( BtU32 iSet, BtFloat x, BtFloat y )
{
	switch( iSet )
	{
		case 0 :

			m_vertexType |= VT_UV0;
			m_v2UV0 = MtVector2( x, y );
			break;

		case 1 :

			m_vertexType |= VT_UV1;
			m_v2UV1 = MtVector2( x, y );
			break;

		default:

			//ErrorLog::Fatal_Printf( "UV set not handled.\r\n" );
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// UV

MtVector2 ExVertex::UV( BtU32 iSet ) const
{
	switch( iSet )
	{
	case 0 :

		return m_v2UV0;
		break;

	case 1 :

		return m_v2UV1;
		break;

	default:

		//ErrorLog::Fatal_Printf( "UV set not handled.\r\n" );
		break;
	}

	return MtVector2( 0, 0 );
}

////////////////////////////////////////////////////////////////////////////////
// Colour

void ExVertex::Colour( BtFloat r, BtFloat g, BtFloat b, BtFloat a )
{
	m_colour = RsColour( r, g, b, a ).asWord();

	m_vertexType |= VT_Colour;
}

void ExVertex::Colour( const RsColour& colour )
{
	m_colour = colour.asWord();

	m_vertexType |= VT_Colour;
}


////////////////////////////////////////////////////////////////////////////////
// Bone

void ExVertex::Bone( BtU32 iIndex, BtFloat fWeight, BtU32 iBone )
{
	m_weight[iIndex] = fWeight;
	m_boneIndices[iIndex] = iBone;

	m_vertexType |= VT_Skin;
	m_vertexType |= VT_Tangent;
	m_vertexType |= VT_Binormal;
}

////////////////////////////////////////////////////////////////////////////////
// GetStride

BtU32 ExVertex::GetStride() const
{
	BtU32 Stride = 0;

	if( m_vertexType & VT_Position )
	{
		Stride += sizeof( MtVector3 ); // 0
	}
	if( m_vertexType & VT_Normal )
	{
		Stride += sizeof( MtVector3 );
	}
	if( m_vertexType & VT_Binormal )
	{
		Stride += sizeof( MtVector3 );
	}
	if( m_vertexType & VT_Tangent )
	{
		Stride += sizeof( MtVector3 );
	}
	if( m_vertexType & VT_Colour )
	{
		Stride += sizeof( BtU32 );
	}
	if( m_vertexType & VT_UV0 )
	{
		Stride += sizeof( MtVector2 );
	}
	if( m_vertexType & VT_UV1 )
	{
		Stride += sizeof( MtVector2 );
	}
	if( m_vertexType & VT_Skin )
	{
		Stride += sizeof( m_weights ) + sizeof( m_boneIndices );
	}
	if( m_vertexType & VT_BlendShape )
	{
		Stride += sizeof( MtVector3 ); // 1
		Stride += sizeof( MtVector3 ); // 2
		Stride += sizeof( MtVector3 ); // 3
	}
	return Stride;
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExVertex::Export( PaMemoryResource& memoryResource )
{
	if( m_vertexType == 0 )
	{
		ErrorLog::Fatal_Printf( "Vertex format contains no elements.\r\n" );
	}

	BtU32 tell1 = memoryResource.Tell();

	if( m_vertexType & VT_Position )
	{
		memoryResource << m_v3Position;	// 0
	}

	if( m_vertexType & VT_Normal )
	{
		memoryResource << m_v3Normal;
	}

	if( m_vertexType & VT_Colour )
	{
		memoryResource << m_colour;
	}

	if( m_vertexType & VT_UV0 )
	{
		memoryResource << m_v2UV0;
	}

	if( m_vertexType & VT_UV1 )
	{
		memoryResource << m_v2UV1;
	}

	if (m_vertexType & VT_Tangent)
	{
		memoryResource << m_v3Normal;
	}

	if (m_vertexType & VT_Binormal)
	{
		memoryResource << m_v3Normal;
	}

	if( m_vertexType & VT_Skin )
	{
		memoryResource << m_weights;
		memoryResource << m_indices;
	}

	if( m_vertexType & VT_BlendShape )
	{
		memoryResource << m_v3Position1; // 1
		memoryResource << m_v3Position2; // 2
		memoryResource << m_v3Position3; // 3
	}

	BtU32 tell2 = memoryResource.Tell();
	BtU32 size = tell2 - tell1;

	BtU32 power2 = BtNearestPowerOf2( size );

	if( PaTopState::Instance().IsVertexBase2() )
	{
		while( tell2 - tell1 < power2 )
		{
			BtU32 pad = 0;
			memoryResource << pad;

			tell2 = memoryResource.Tell();
		}

		tell2 = memoryResource.Tell();
		size = tell2 - tell1;
		(void)size;

		int a=0;
		a++;
	}
}

void ExVertex::CalculateHashCode()
{
	m_crc = 0;		// Ensure the result is 0 to begin with as this is in the hashcode

	m_crc = BtCRC::GenerateHashCode( (const BtU8*) this, sizeof( ExVertex ) );
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExVertex::ChangeCoordinateSystem()
{
	m_v3Position.z = -m_v3Position.z;
	m_v3Normal.z = -m_v3Normal.z;

	BtAssert( MaxUVSets == 8 );

	if( m_invertUV == BtTrue )
	{
		if( m_vertexType & VT_UV0 )
		{
			m_v2UV0.x =  m_v2UV0.x;
			m_v2UV0.y = -m_v2UV0.y;
		}

		if( m_vertexType & VT_UV1 )
		{
			m_v2UV1.x =  m_v2UV1.x;
			m_v2UV1.y = -m_v2UV1.y;
		}
	}
	else
	{
		if( m_vertexType & VT_UV0 )
		{
			m_v2UV0.x = m_v2UV0.x;
			m_v2UV0.y = m_v2UV0.y;
		}

		if( m_vertexType & VT_UV1 )
		{
			m_v2UV1.x = m_v2UV1.x;
			m_v2UV1.y = m_v2UV1.y;
		}
	}
}

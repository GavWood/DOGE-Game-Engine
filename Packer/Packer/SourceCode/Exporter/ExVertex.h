////////////////////////////////////////////////////////////////////////////////
// ExVertex.h

#pragma once
#include "BtTypes.h"
#include "BtCRC.h"
#include "PaMemoryResource.h"
#include "RsVertex.h"
#include "RsColour.h"
#include "BaFileData.h"

const BtU32 MaxUVSets = 8;

class ExVertexBoneWeights
{
	BtFloat									m_weights[4];
};

class ExVertexBoneIndex
{
	BtS8									m_index[4];
};

class ExVertex
{
public:
	
	ExVertex();
	
	void									Export( PaMemoryResource& memoryResource );
	void									ChangeCoordinateSystem();
	void									CalculateHashCode();

	// Accessors
	BtBool									operator < ( const ExVertex& vertex ) const;

	BtChar*									Description() const;

	BtU32									GetVertexType() const;

	BtU32									GetStride() const;
	const MtVector3& 						Position() const;
	const MtVector3& 						Normal() const;
	MtVector2		 						UV( BtU32 iSet ) const;
	void 									Position( BtFloat X, BtFloat Y, BtFloat Z );
	void 									Position( const MtVector3& Position );

	void 									Position1( const MtVector3& Position );
	void 									Position2( const MtVector3& Position );
	void 									Position3( const MtVector3& Position );

	void 									Normal( BtFloat X, BtFloat Y, BtFloat Z );
	void 									Tangent( BtFloat X, BtFloat Y, BtFloat Z );
	void 									Binormal( BtFloat X, BtFloat Y, BtFloat Z );
	void 									UV( BtU32 iSet, BtFloat X, BtFloat Y );
	void 									Colour( BtFloat r, BtFloat g, BtFloat b, BtFloat a );
	void 									Colour( const RsColour& colour );
	void 									Bone( BtU32 iIndex, BtFloat fWeight, BtU32 iBone );
	void 									Bone( BtU32 iIndex, BtU32 iBone );
	BtS32									Bone( BtU32 iIndex ) const;

	BtU32									CRC() const;

private:

	BtU32									m_vertexType;
	BtU32									m_crc;

	// Vertex data
	MtVector3								m_v3Position;
	MtVector3								m_v3Position1;
	MtVector3								m_v3Position2;
	MtVector3								m_v3Position3;
	MtVector3								m_v3Normal;
	MtVector3								m_v3Tangent;
	MtVector3								m_v3Bionormal;
	BtU32									m_colour;
	MtVector2								m_v2UV0;
	MtVector2								m_v2UV1;
	union
	{
		BtFloat								m_weight[4];
		ExVertexBoneWeights					m_weights;
	};
	union
	{
		BtS8								m_boneIndices[4];
		ExVertexBoneIndex					m_indices;
	};

	BtBool									m_invertUV;
};

inline BtBool ExVertex::operator < ( const ExVertex& vertex ) const
{
	return m_crc < vertex.CRC();
}

////////////////////////////////////////////////////////////////////////////////
// CRC

inline BtU32 ExVertex::CRC() const
{
	return m_crc;
}

////////////////////////////////////////////////////////////////////////////////
// Position

inline const MtVector3& ExVertex::Position() const
{
	return m_v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// Normal

inline const MtVector3& ExVertex::Normal() const
{
	return m_v3Normal;
}

////////////////////////////////////////////////////////////////////////////////
// Bone

inline void ExVertex::Bone( BtU32 iIndex, BtU32 iBone )
{
	m_boneIndices[iIndex] = iBone;
}

inline BtS32 ExVertex::Bone( BtU32 iIndex ) const
{
	return m_boneIndices[iIndex];
}

////////////////////////////////////////////////////////////////////////////////
// VertexType

inline BtU32 ExVertex::GetVertexType() const
{
	BtChar* pDescription = Description();
	
	(void) pDescription;

	return m_vertexType;
}

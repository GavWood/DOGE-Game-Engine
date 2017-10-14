////////////////////////////////////////////////////////////////////////////////
// ExMaterial

#pragma once
#include "BaArchive.h"
#include "BtTypes.h"
#include "ExResource.h"
#include "ExTextureList.h"

const BtU32 MaxMaterialName = 64;
class ExTexture;
struct ExSgNode;

const BtU32 DiffuseTextureIndex  = 0;
const BtU32 NormalMapIndex		 = 1;
const BtU32 LightMapIndex		 = 2;
const BtU32 DiffuseMaskIndex	 = 3;
const BtU32 SpecularTextureIndex = 4;

class ExMaterial : public ExResource
{
public:

	ExMaterial();

	void								Export();

	// Accessors
	BtChar*								GetName();
	void								SetName( BtChar* pName );
	void								DiffuseColour( RsColour& colour );
	const RsColour&						DiffuseColour() const;
	void								VertexType( BtU32 vertexType );
	void								BlendShape( BtBool blendShape );
	void								SetFlag( BtU32 flag );
	void								SetTextureIndex( BtU32 ID, ExTexture* pTexture );

	BtFloat								GetRepeatU();
	BtFloat								GetRepeatV();
	void								SetRepeatU( BtFloat repeatU );
	void								SetRepeatV( BtFloat repeatV );

	BtFloat								GetOffsetU();
	BtFloat								GetOffsetV();
	void								SetOffsetU( BtFloat offsetU );
	void								SetOffsetV( BtFloat offsetV );

private:

	void								SetTechniqueName( BtChar* pTechniqueName );
	void								Destroy();

	BtChar								m_name[MaxMaterialName];
	BaMaterialFileData					m_fileData;
	ExTextureList						m_textureList;
	BtArray<ExTexture*, MaxTextures>	m_textures;
	BtFloat								m_repeatU;
	BtFloat								m_repeatV;
	BtFloat								m_offsetU;
	BtFloat								m_offsetV;
};

////////////////////////////////////////////////////////////////////////////////
// DiffuseColour

inline void ExMaterial::DiffuseColour( RsColour& diffuseColour )
{
	m_fileData.m_diffuseColour = diffuseColour;
}

////////////////////////////////////////////////////////////////////////////////
// DiffuseColour

inline const RsColour& ExMaterial::DiffuseColour() const
{
	return m_fileData.m_diffuseColour;
}

////////////////////////////////////////////////////////////////////////////////
// GetName

inline BtChar* ExMaterial::GetName()
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////
// SetName

inline void ExMaterial::SetName( BtChar* pName )
{
	BtStrCopy( m_name, MaxMaterialName, pName );
}

////////////////////////////////////////////////////////////////////////////////
// VertexType

inline void ExMaterial::VertexType( BtU32 vertexType )
{
	m_fileData.m_vertexType = vertexType;
}

////////////////////////////////////////////////////////////////////////////////
// BlendShape

inline void ExMaterial::BlendShape( BtBool blendShape )
{
	m_fileData.m_flags |= RsMaterial_BlendShape;
}

inline void ExMaterial::SetFlag( BtU32 flag )
{
	m_fileData.m_flags |= flag;
}

////////////////////////////////////////////////////////////////////////////////
// SetRepeatU

inline void ExMaterial::SetRepeatU( BtFloat repeatU )
{
	m_repeatU = repeatU;
}

////////////////////////////////////////////////////////////////////////////////
// SetRepeatV

inline void ExMaterial::SetRepeatV( BtFloat repeatV )
{
	m_repeatV = repeatV;
}

////////////////////////////////////////////////////////////////////////////////
// GetRepeatU

inline BtFloat ExMaterial::GetRepeatU()
{
	return m_repeatU;
}

////////////////////////////////////////////////////////////////////////////////
// GetRepeatV

inline BtFloat ExMaterial::GetRepeatV()
{
	return m_repeatV;
}

////////////////////////////////////////////////////////////////////////////////
// SetOffsetU

inline void ExMaterial::SetOffsetU( BtFloat OffsetU )
{
	m_offsetU = OffsetU;
}

////////////////////////////////////////////////////////////////////////////////
// SetOffsetV

inline void ExMaterial::SetOffsetV( BtFloat OffsetV )
{
	m_offsetV = OffsetV;
}

////////////////////////////////////////////////////////////////////////////////
// GetOffsetU

inline BtFloat ExMaterial::GetOffsetU()
{
	return m_offsetU;
}

////////////////////////////////////////////////////////////////////////////////
// GetOffsetV

inline BtFloat ExMaterial::GetOffsetV()
{
	return m_offsetV;
}

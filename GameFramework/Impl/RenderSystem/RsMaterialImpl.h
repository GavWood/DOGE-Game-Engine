////////////////////////////////////////////////////////////////////////////////
/// RsMaterialImpl.h

#pragma once
#include "BaFileData.h"
#include "RsMaterial.h"
#include "RsTextureImpl.h"
#include "RsRenderTarget.h"

struct RsMaterialRenderable;

// Class Declaration
class RsMaterialImpl : public RsMaterial
{
public:

	RsMaterialImpl();

	RsMaterial*				GetDuplicate() const;

	void					FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	void					Render( const MtVector2& v2Position,
									const RsColour& colour,
									BtU32 nSortOrder,
									BtFloat fRotation = 0 );

	// Primitive rendering for the programmer
	void					Render( RsPrimitiveType primitiveType, RsVertex2 *pVertex, BtU32 nVertex, BtU32 sortOrder );
	void					Render( RsPrimitiveType primitiveType, RsVertex3 *pVertex, BtU32 nVertex, BtU32 sortOrder, BtBool isCopy = BtTrue );
	void					Render( const MtVector2& v2Position, const RsColour &colour, BtU32 sortOrder );
	void					Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, const RsColour &colour, BtU32 sortOrder );
	void					Render( RsMaterialRenderable *pRenderable );

	// Accessors
	BtChar*					GetTechniqueName() const;
	void					SetTechniqueName( const BtChar* pName );
    
    void                    SetZTest( BtBool isEnabled );
    BtBool                  GetZTest();

	BtU32					Width();
	BtU32					Height();

	MtVector2				GetDimension();

	const RsColour&			GetDiffuseColour() const;
	void					SetDiffuseColour( const RsColour& colour );
	
	RsTexture*				GetTexture( BtU32 iTexture );
	void					SetTexture( BtU32 iTexture, RsTexture* pTexture );

	BtU32					GetFlags() const;
	BtU32					GetPasses() const;
	BtU32					GetSortOrder() const;
	BtU32					GetVertexType() const;
	
	void					SetSortOrder( BtU32 sortOrder ) const;

private:

	friend class SgMaterialsWin32;
	friend class SgMaterialsWinGL;

	BaMaterialFileData*		m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// GetDiffuseColour

inline const RsColour& RsMaterialImpl::GetDiffuseColour() const
{
	return m_pFileData->m_diffuseColour;
}

////////////////////////////////////////////////////////////////////////////////
// SetDiffuseColour

inline void RsMaterialImpl::SetDiffuseColour( const RsColour& colour )
{
	m_pFileData->m_diffuseColour = colour;
}

////////////////////////////////////////////////////////////////////////////////
// pTexture

inline RsTexture* RsMaterialImpl::GetTexture( BtU32 iTextureIndex )
{
    return (RsTexture*) m_pFileData->m_pTexture[iTextureIndex];
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

inline MtVector2 RsMaterialImpl::GetDimension()
{
	return MtVector2( (BtFloat)Width(), (BtFloat)Height() );
}

////////////////////////////////////////////////////////////////////////////////
// SetTexture

inline void RsMaterialImpl::SetTexture( BtU32 iTextureIndex, RsTexture* pTexture )
{
	m_pFileData->m_pTexture[iTextureIndex] = (RsTextureImpl*) pTexture;
}

////////////////////////////////////////////////////////////////////////////////
// GetSortOrder

inline BtU32 RsMaterialImpl::GetSortOrder() const
{
	return m_pFileData->m_nSortOrder;
}

////////////////////////////////////////////////////////////////////////////////
// SetSortOrder

inline void RsMaterialImpl::SetSortOrder( BtU32 sortOrder ) const
{
	m_pFileData->m_nSortOrder = sortOrder;
}

////////////////////////////////////////////////////////////////////////////////
// VertexType

inline BtU32 RsMaterialImpl::GetVertexType() const
{
	return m_pFileData->m_vertexType;
}

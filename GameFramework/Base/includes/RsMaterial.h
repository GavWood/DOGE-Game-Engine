////////////////////////////////////////////////////////////////////////////////
/// RsMaterial.h

#pragma once
#include "BaArchive.h"
#include "BaResource.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsVertex.h"
#include "RsRenderTypes.h"

// Class Declaration
class RsMaterial : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Material );

	virtual RsMaterial*			GetDuplicate() const = 0;

	// Accessors
	virtual BtChar*				GetTechniqueName() const = 0;
	virtual void				SetTechniqueName( const BtChar* pName ) = 0;

	virtual const RsColour&		GetDiffuseColour() const = 0;
	virtual void				SetDiffuseColour( const RsColour& colour ) = 0;

	virtual void				SetZTest(BtBool enabled) = 0;
	virtual BtBool				GetZTest() = 0;

	virtual RsTexture*			GetTexture( BtU32 iTexture ) = 0;
	virtual void				SetTexture( BtU32 iTexture, RsTexture* pTexture ) = 0;

	BtFloat						GetAspect() { return (BtFloat)Width() / (BtFloat)Height(); }

	virtual BtU32				Width() = 0;
	virtual BtU32				Height() = 0;
   
	virtual MtVector2			GetDimension() = 0;

	virtual BtU32				GetPasses() const = 0;
	virtual BtU32				GetSortOrder() const = 0;
	virtual void				SetSortOrder( BtU32 sortOrder ) const = 0;

    virtual void				Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, const RsColour &colour, BtU32 sortOrder ) = 0;

	virtual void				Render( RsPrimitiveType primitiveType, RsVertex2 *pVertex, BtU32 nVertex, BtU32 sortOrder ) = 0;
	
	virtual void				Render( RsPrimitiveType primitiveType, RsVertex3 *pVertex, BtU32 nVertex, BtU32 sortOrder, BtBool isVertexCopied = BtTrue) = 0;

	virtual void				Render( const MtVector2& v2Position, const RsColour &colour, BtU32 sortOrder ) = 0;
};

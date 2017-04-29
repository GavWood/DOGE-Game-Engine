////////////////////////////////////////////////////////////////////////////////
/// RsFont.h

#pragma once
#include "BaResource.h"
#include "MtVector2.h"
#include "RsColour.h"

class RsTrueTypeFont
{
public:

	virtual void Render(const BtChar *text, RsColour colour) = 0;
};

// Class Declaration
class RsFont : public BaResource
{
	DECLARE_RESOURCETYPE( BaRT_Font );

public:

	virtual MtVector2 GetDimension( const BtChar* szText ) = 0;
	
	virtual void      SetNewLineRatio( BtFloat ratio ) = 0;

	virtual void	  Render( const MtVector2 &v2Position,
							  const MtVector2 &v2Dimension,
							  const RsColour &colour,
					 	 	  const BtChar* szText,
						 	  BtU32 sortOrder,
							  RsTexture *pTexture,
							  RsTrueTypeFont *pFont ) = 0;

	virtual MtVector2 Render( const MtVector2& v2Position,
							  const RsColour &colour,
							  const BtChar* szText,
							  BtU32 sortOrder) = 0;

	virtual MtVector2 Render( const MtVector2& v2Position,
							  BtFloat scale,
							  const RsColour &colour,
							  const BtChar* szText,
							  BtU32 sortOrder ) = 0;
};

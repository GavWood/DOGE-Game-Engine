////////////////////////////////////////////////////////////////////////////////
// RsSprite.h

#pragma once
#include "BaResource.h"
#include "BaArchive.h"
#include "RsColour.h"
#include "MtVector2.h"

// Class declaration
class RsSprite : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Sprite );

	virtual void			FixPointers( BtU8 *pFileData, BaArchive *pArchive ) = 0;
	virtual void 			CreateOnDevice() = 0;
	virtual void 			RemoveFromDevice() = 0;
	virtual void			Render( BtBool isFlipX, const MtVector2& v2Position, BtU32 spriteIndex, BtFloat rotation, BtU32 sortOrder ) = 0;
	virtual void			Render( BtBool isFlipX, const MtVector2& v2Position, BtU32 spriteIndex, BtFloat rotation, const RsColour& colour, BtU32 sortOrder ) = 0;
	virtual void			Render( BtBool isFlipX, const MtVector2& v2Position, const MtVector2& v2Dimension, BtU32 spriteIndex, BtFloat rotation, const RsColour& colour, BtU32 sortOrder ) = 0;
	virtual BtU32			GetNumSprites() const = 0;
	virtual MtVector2		GetDimension( BtU32 iSprite ) const = 0;
};

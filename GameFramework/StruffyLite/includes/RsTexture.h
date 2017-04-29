////////////////////////////////////////////////////////////////////////////////
// RsTexture.h

#pragma once
#include "BaResource.h"
#include "RsColour.h"
#include "BaArchive.h"
#include "MtVector2.h"

// Class declaration
class RsTexture : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Texture );

	virtual BtU32			GetTextureHandle() = 0;
	virtual void			FixPointers( BtU8 *pFileData, BaArchive *pArchive ) = 0;
	virtual void 			CreateOnDevice() = 0;
	virtual void 			RemoveFromDevice() = 0;
	virtual RsTexture*		GetDuplicate() const = 0;

	virtual void			SetClamped(BtBool isEnabled) = 0;
	virtual void			SetWrapped(BtBool isEnabled) = 0;

	virtual BtU32			GetWidth() const = 0;
	virtual BtU32			GetHeight() const = 0;
	virtual MtVector2		GetDimension() const = 0;

	virtual BtU32			GetOriginalWidth() const = 0;
	virtual BtU32			GetOriginalHeight() const = 0;
	virtual MtVector2		GetOriginalDimension() const = 0;

	virtual BtU8		   *ReadMemory() = 0;
	virtual void			WriteMemory( BtU8* pMemory, BtU32 size ) = 0;

	virtual RsColour		GetPixel( BtU32 x, BtU32 y, BtU8 &red, BtU8 &green, BtU8 &blue, BtU8 &alpha ) = 0;

	virtual void			Render( const MtVector2& v2Position, BtU32 sortOrder ) = 0;
	virtual void			Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, BtU32 sortOrder ) = 0;
	virtual void			Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, RsColour colour, BtU32 sortOrder ) = 0;
};

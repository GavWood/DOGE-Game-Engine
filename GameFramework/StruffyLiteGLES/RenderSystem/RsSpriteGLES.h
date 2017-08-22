////////////////////////////////////////////////////////////////////////////////
// RsSpriteWin32GL.h

#ifndef __RsSpriteWin32GL_h__
#define __RsSpriteWin32GL_h__

// Includes
#include "BaResource.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "RsColour.h"
#include "RsSprite.h"
#include "MtVector2.h"

class RsSprite;
struct RsSpriteRenderable;

// Class declaration
class RsSpriteWinGL : public RsSprite
{
public:

	void					FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	BtU32					GetNumSprites() const;
	MtVector2				GetDimension( BtU32 spriteNum ) const;
	void 					CreateOnDevice();
	void 					RemoveFromDevice();

	void					Render( BtBool isFlipX, const MtVector2& v2Position, BtU32 spriteIndex, BtFloat rotation, BtU32 sortOrder );
	void					Render( BtBool isFlipX, const MtVector2& v2Position, BtU32 spriteIndex, BtFloat rotation, const RsColour& colour, BtU32 sortOrder );
	void					Render( BtBool isFlipX, const MtVector2& v2Position, const MtVector2& v2Dimension, BtU32 spriteIndex, BtFloat rotation, const RsColour& colour, BtU32 sortOrder );
	void					Render( RsSpriteRenderable *pRenderable );

private:

	LBaSpriteFileData*		m_pFileData;
};

#endif // __RsSpriteWin32GL_h__

////////////////////////////////////////////////////////////////////////////////
// RsTextureWin32GL.h

#pragma once
#include "BaResource.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "RsTexture.h"
#include "RsColour.h"
#include "MtVector2.h"
#include "RsPlatform.h"

struct RsTextureRenderable;

// Class declaration
class RsTextureWinGL : public RsTexture
{
public:

	virtual BtU32			GetTextureHandle();
	virtual void			FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	
	RsTexture*				GetDuplicate() const { return BtNull; };

	void 					CreateOnDevice();
	void 					RemoveFromDevice();

	BtU32					GetWidth() const;
	BtU32					GetHeight() const;
	virtual MtVector2		GetDimension() const;

	virtual BtU32			GetOriginalWidth() const;
	virtual BtU32			GetOriginalHeight() const;
	virtual MtVector2		GetOriginalDimension() const;

	virtual BtU8		   *ReadMemory();
	virtual void			WriteMemory( BtU8* pMemory, BtU32 size );

	void					SetClamped(BtBool isEnabled);
	void					SetWrapped(BtBool isEnabled);

	void					SetTexture();

	RsColour				GetPixel( BtU32 x, BtU32 y, BtU8& red, BtU8& green, BtU8& blue, BtU8 &alpha );

	void					Render( const MtVector2& v2Position, BtU32 sortOrder );
	
	void					Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, RsColour colour, BtU32 sortOrder );

	void					Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, BtU32 sortOrder );

	void					Render( RsTextureRenderable *pRenderable );

private:

	friend class RsRenderTargetWinGL;

	LBaTextureFileData*		m_pFileData;
	LBaMipMapFileData*		m_pMipmaps;

	GLenum					checkError( const BtChar *error );

	BtU32					m_texture;
	GLuint					m_frameBufferObject;
	GLuint					m_renderBufferObject;
};

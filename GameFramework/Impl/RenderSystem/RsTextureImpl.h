////////////////////////////////////////////////////////////////////////////////
// RsTextureImpl.h

#pragma once

// Includes
#include "BaResource.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "RsTexture.h"
#include "RsColour.h"
#include "MtVector2.h"
#include "RsPlatform.h"

struct RsTextureRenderable;

// Class declaration
class RsTextureImpl : public RsTexture
{
public:

    BtU32                   GetTextureHandle();
    
	void		        	FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	void 					CreateOnDevice();
	void 					RemoveFromDevice();
    
    BtU32                   GetWindowHandle();
    
    RsTexture*		        GetDuplicate() const;

	BtU32					GetWidth() const;
	BtU32					GetHeight() const;
	MtVector2		        GetDimension() const;

    void		        	SetClamped(BtBool isEnabled) {};
    void		        	SetWrapped(BtBool isEnabled) {};
    
	BtU32		        	GetOriginalWidth() const;
	BtU32		        	GetOriginalHeight() const;
	MtVector2	        	GetOriginalDimension() const;

    BtU8                   *ReadMemory();
    void                    WriteMemory( BtU8* pMemory, BtU32 size );

    void                    SetTexture( BtU32 newHandle );

	RsColour				GetPixel( BtU32 x, BtU32 y, BtU8& red, BtU8& green, BtU8& blue, BtU8 &alpha );

	void					Render( const MtVector2& v2Position, BtU32 sortOrder );
	
	void					Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, RsColour colour, BtU32 sortOrder );

	void					Render( const MtVector2& v2Position, const MtVector2 &v2Dimension, BtU32 sortOrder );

	void					Render( RsTextureRenderable *pRenderable );

private:

	friend class RsRenderTargetWinGL;

	LBaTextureFileData*		m_pFileData;
	LBaMipMapFileData*		m_pMipmaps;

	BtU32					m_texture;
	GLuint					m_frameBufferObject;
	GLuint					m_renderBufferObject;
};


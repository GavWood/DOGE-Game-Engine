////////////////////////////////////////////////////////////////////////////////
/// RsFontWin32GL.h

#pragma once
#include "RsFont.h"
#include "RsTextureImpl.h"

struct LBaFontFileData;
class BaArchive;
class RsFont;
class RsMaterial;
struct RsFontRenderable;

// Class Declaration
class RsFontImpl : public RsFont
{
public:

	void				FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	MtVector2			GetDimension( const BtChar* szText );

	void				SetNewLineRatio( BtFloat ratio );

    
    void				Render( const MtVector2 &v2Position,
                               const MtVector2 &v2Dimension,
                               const RsColour &colour,
                               const BtChar* szText,
                               BtU32 sortOrder,
                               RsTexture *pTexture,
                               RsTrueTypeFont *pFont );
    
	MtVector2			Render( const MtVector2& v2Position,
								const RsColour &colour,
								const BtChar* szText,
								BtU32 sortOrder );

	MtVector2			Render( const MtVector2& v2Position,
								BtFloat scale,
                                const RsColour &colour,
								const BtChar* szText,
								BtU32 sortOrder );

	void				Render( RsFontRenderable *pFontRenderable );

private:

	friend class BaArchiveManager;
	friend class RcRenderPipeline;

	LBaFontFileData*	m_pFileData;
	RsTextureImpl*		m_pTextures[MaxFontTexturePages];
	RsMaterial*			m_pMaterial;
	BtFloat				m_ratio;
};

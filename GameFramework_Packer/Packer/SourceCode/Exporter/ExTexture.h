////////////////////////////////////////////////////////////////////////////////
// ExTexture.h

#pragma once
#include "PaAsset.h"
#include "BaArchive.h"
#include "ExResource.h"
#include "BaFileData.h"

class ExTexture : public ExResource
{
public:

	ExTexture( BtBool isInverted );
	ExTexture();

	void									Export();

	// Accessors
	void									SetInverted( BtBool isInverted );

	void									SetFlipped( BtBool isFlipped );

	void									IsMipMapped( BtBool mipMaps );

	void									SetClampU( BtBool isClamp );
	void									SetClampV( BtBool isClamp);

	void									SetWrapU( BtBool isWrap);
	void									SetWrapV( BtBool isWrap);

	BtBool									IsAlpha() const;

private:

	LBaTextureFileData						m_fileData;
	BtBool									m_isInverted;
	BtBool									m_isFlipped;
	BtBool									m_isWrapU;
	BtBool									m_isWrapV;
	BtBool									m_isMipmapped;
	BtBool									m_isAlpha;
	BtU32									m_resourceID;
	BtChar									m_maskFilename[256];
};

////////////////////////////////////////////////////////////////////////////////
// bInvert

inline void ExTexture::SetInverted( BtBool bInvert )
{
	m_isInverted = bInvert;
}

////////////////////////////////////////////////////////////////////////////////
// SetFlipped

inline void ExTexture::SetFlipped( BtBool isFlipped )
{
	m_isFlipped = isFlipped;
}

////////////////////////////////////////////////////////////////////////////////
// IsMipMapped

inline void ExTexture::IsMipMapped( BtBool mipMaps )
{
	m_isMipmapped = mipMaps;
}

////////////////////////////////////////////////////////////////////////////////
// IsAlpha

inline BtBool ExTexture::IsAlpha() const
{
	return m_isAlpha;
}

////////////////////////////////////////////////////////////////////////////////
// SetClampU

inline void ExTexture::SetClampU( BtBool isClamp)
{
}

////////////////////////////////////////////////////////////////////////////////
// SetClampV

inline void ExTexture::SetClampV( BtBool isClamp)
{
}

inline void ExTexture::SetWrapU( BtBool isWrap )
{
	m_isWrapU = isWrap;
}

inline void ExTexture::SetWrapV( BtBool isWrap)
{
	m_isWrapV = isWrap;
}

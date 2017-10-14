////////////////////////////////////////////////////////////////////////////////
// ExTextureList

#ifndef __ExTextureList_h__
#define __ExTextureList_h__

#include "BaArchive.h"
#include "BtTypes.h"
#include "ExTexture.h"
#include <vector>

struct ExTextureList
{
public:

	ExTexture*						pFind( const BtChar* pFilename );
	ExTexture*						pTexture( BtU32 nTexture );
	ExTexture*						Add( ExTexture* pTexture );
	BtU32							nTextures();

private:

	std::vector<ExTexture*>		m_pTextures;
};

////////////////////////////////////////////////////////////////////////////////
// nTextures

inline BtU32 ExTextureList::nTextures()
{
	return (BtU32) m_pTextures.size();
}

#endif // __ExTextureList_h__
////////////////////////////////////////////////////////////////////////////////
// ExTextureList.cpp

#include "ExExporter.h"
#include "ExTextureList.h"
#include "FsFile.h"
#include "PaMemoryResource.h"
#include "PaArchive.h"

////////////////////////////////////////////////////////////////////////////////
// pFind

ExTexture* ExTextureList::pFind( const BtChar* pFilename )
{
	BtU32 nTextureCount = (BtU32) m_pTextures.size();

	for( BtU32 nTexture=0; nTexture<nTextureCount; nTexture++ )
	{
		// Cache each Texture
		ExTexture* pTexture = m_pTextures[nTexture];

		if( BtStrCompare( pTexture->GetFilename(), pFilename ) == BtTrue )
		{
			return pTexture;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// pTexture

ExTexture* ExTextureList::pTexture( BtU32 nTexture )
{
	return m_pTextures[nTexture];
}

////////////////////////////////////////////////////////////////////////////////
// Add

ExTexture* ExTextureList::Add( ExTexture* pTexture )
{
	// Add the texture as a dependency
	ExExporter::Instance().pCurrentAsset()->AddDependency( pTexture->GetFilename() );

	for( BtU32 i=0; i<m_pTextures.size(); i++ )
	{
		ExTexture* pCurrentTexture = m_pTextures[i];

		if( BtStrCompare( pCurrentTexture->GetFilename(), pTexture->GetFilename() ) == BtTrue )
		{
			return pCurrentTexture;
		}
	}

	// Export this texture
	pTexture->Export();

	// Add the texture to the list
	m_pTextures.push_back( pTexture );

	return pTexture;
}


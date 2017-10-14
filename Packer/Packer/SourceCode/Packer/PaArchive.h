////////////////////////////////////////////////////////////////////////////////
// PaArchive

#ifndef __PaArchive_h__
#define __PaArchive_h__

#include "PaFileDetails.h"
#include "PaAsset.h"
#include "PaDependency.h"
#include "PaMemoryResource.h"
#include "ExSchedule.h"

class PaArchive
{
public:

	 PaArchive();
	~PaArchive();

	PaResource*						AddResource( PaResource* pResource );

	BtU32 							AddResource( PaAsset* pAsset,
												 PaMemoryResource& memoryResource,
												 const BtChar* pFilename,
												 const BtChar* pTitle,
												 BaResourceType eType );

	PaResource*						GetResource( BtU32 nIndex );

	// Write the archive
	void							WriteArchive();
	void							WriteResourceSizes();
	BtBool							isDependency( const BtChar* pFilename );
	void							RemoveOldDependencies();

	// Accessors for files found in this archive
	void							AddFile( const PaFileDetails& file );
	const PaFileDetails*			GetFile( const BtChar* pFilename );
	const PaFileDetails*			GetFile( BtU32 nIndex );
	BtU32							nFiles() const;

	// Accessors for assets in this archive
	void							AddAsset( PaAsset& asset );
	PaAsset*						GetAsset( BtU32 nIndex );
	BtU32							GetNumAssets() const;
	BtU32							GetNumResources() const;

	// Accessors
	const BtChar*					pTitle() const;

	// Members
	BaArchiveHeader				m_archiveHeader;
	PaFileDetails					m_file;								// File information for the archive

	BtChar							m_szArchiveFilename[MaxFilePath];	// Archive filename
	BtChar							m_szPackedFilename[MaxFilePath];	// Archive packed filename

	BtU32							m_archiveSize;

private:

	std::vector<PaAsset>			m_assets;							// List of assets in this archive
	std::vector<PaResource*>		m_resources;						// List of resources in this archive
	std::vector<PaFileDetails>		m_files;							// List of files in this archive
};

#endif // __PaArchive_h__

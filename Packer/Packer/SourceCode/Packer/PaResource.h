////////////////////////////////////////////////////////////////////////////////
// PaResource.h

#pragma once
#include <vector>
#include "BaArchive.h"
#include "BtTypes.h"
#include "BtArray.h"
#include "PaFilename.h"
#include "BaArchive.h"

struct BaResourceHeader;
class PaAsset;
class PaArchive;

class PaResource
{
public:

	PaResource();

	PaResource( PaAsset* pAsset,
			    const BtChar* pFilename,
				const BtChar* pTitle,
				BaResourceType eType );

	PaResource( PaAsset* pAsset,
				const BtChar* pTitle,
				BaResourceType eType );


	// Accessors
	BtChar*							compiledFilename();
	BtChar*							uncompiledFilename();

	BaResourceHeader*				GetHeader();
	void							SetHeader( const BaResourceHeader& header );

	PaArchive*						pArchive();

	BtChar*							pArchiveName();

private:

	BtChar							m_compiledFilename[MaxFilePath];
	BtChar							m_uncompiledFilename[MaxFilePath];
	BaResourceHeader				m_resourceHeader;

	PaArchive*						m_pArchive;
	BtChar							m_archive[MaxFilePath];
};

////////////////////////////////////////////////////////////////////////////////
// pArchiveName

inline BtChar* PaResource::pArchiveName()
{
	return m_archive;
}

////////////////////////////////////////////////////////////////////////////////
// pArchive

inline PaArchive* PaResource::pArchive()
{
	return m_pArchive;
}

////////////////////////////////////////////////////////////////////////////////
// compiledFilename

inline BtChar* PaResource::compiledFilename()
{
	return m_compiledFilename;
}

////////////////////////////////////////////////////////////////////////////////
// uncompiledFilename

inline BtChar* PaResource::uncompiledFilename()
{
	return m_uncompiledFilename;
}

////////////////////////////////////////////////////////////////////////////////
// GetHeader

inline BaResourceHeader* PaResource::GetHeader()
{
	return &m_resourceHeader;
}

////////////////////////////////////////////////////////////////////////////////
// SetHeader

inline void PaResource::SetHeader( const BaResourceHeader& header )
{
	m_resourceHeader = header;
}

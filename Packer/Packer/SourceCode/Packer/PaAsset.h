////////////////////////////////////////////////////////////////////////////////
// PaAsset.h

#pragma once
#include <vector>
#include "BtTypes.h"
#include "BtString.h"
#include "PaArray.h"
#include "PaFileDetails.h"
#include "PaResource.h"

const BtU32 MaxDependencies = 256;

class PaArchive;

class PaAsset
{
public:

	PaAsset( PaArchive* pArchive,
			 BaResourceType eResourceType,
			 const PaFileDetails& file );

	void									AddDependency( const BtChar* pFile );
	const PaFilename&						GetDependency( BtU32 nIndex );
	BtU32									GetNumDependencies();
	
	PaArchive*								pArchive();

	PaFileDetails							m_file;							// File details
																			// i.e. Runtime structure
	BtU64									m_lastExportTime;					// Time of last export
	BtBool									m_bExport;						// Export this time?
	BtBool									m_bDLL;							// Was it compiled with the DLL?

	PaArray<PaFilename, MaxDependencies>	m_dependencies;

	// Accessors
	const PaFileDetails&					File() const;
	BaResourceType							Type();
	BtBool									bExported();
	void									bExported( BtBool bExported );

private:

	friend class PaResource;

	PaArchive*								m_pArchive;
	BaResourceType							m_type;
	BtBool									m_bExported;
};

////////////////////////////////////////////////////////////////////////////////
// Type

inline BaResourceType PaAsset::Type()
{
	return m_type;
}

////////////////////////////////////////////////////////////////////////////////
// File

inline const PaFileDetails& PaAsset::File() const
{
	return m_file;
}

////////////////////////////////////////////////////////////////////////////////
// nDependencies

inline BtU32 PaAsset::GetNumDependencies()
{
	return m_dependencies.nItems();
}

////////////////////////////////////////////////////////////////////////////////
// pArchive

inline PaArchive* PaAsset::pArchive()
{
	return m_pArchive;
}

////////////////////////////////////////////////////////////////////////////////
// Dependency

inline const PaFilename& PaAsset::GetDependency( BtU32 nIndex )
{
	return m_dependencies[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// bExported

inline BtBool PaAsset::bExported()
{
	return m_bExported;
}

////////////////////////////////////////////////////////////////////////////////
// bExported

inline void PaAsset::bExported( BtBool bExported )
{
	m_bExported = bExported;
}

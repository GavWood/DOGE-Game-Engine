////////////////////////////////////////////////////////////////////////////////
// PaAsset.cpp

// Includes
#include <stdio.h>
#include <string.h>
#include "BtMemory.h"
#include "PaAsset.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaAsset::PaAsset( PaArchive* pArchive,
				  BaResourceType eResourceType,
				  const PaFileDetails& file )
{
	// Clear the dependencies
	m_dependencies.Empty();

	// Set the file information
	m_file = file;

	// Flag nothing for export as a default
	m_bExport = BtFalse;

	// Flag nothing for export by DLL
	m_bDLL = BtFalse;

	// Set the archive
	m_pArchive = pArchive;

	// Set the resource type
	m_type = eResourceType;

	// Flag as not exported
	m_bExported = BtFalse;
}


////////////////////////////////////////////////////////////////////////////////
// AddDependency

void PaAsset::AddDependency( const BtChar* pFile )
{
	PaFilename &file = m_dependencies.Add();

	file = PaFilename( pFile );
}
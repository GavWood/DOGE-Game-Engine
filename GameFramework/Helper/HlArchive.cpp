////////////////////////////////////////////////////////////////////////////////
// HlArchive.cp

// Includes
#include "HlArchive.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "ApConfig.h"
#include "RsTexture.h"
#include "RsMaterial.h"
#include "BaArchive.h"
#include "ErrorLog.h"

////////////////////////////////////////////////////////////////////////////////
// Load

void HlArchive::Load()
{
	Unload();

	// Load the archive and flag the archive as loaded
	m_archive.Load( m_archiveName );
    
	// Flag the archive as loaded
	m_isLoaded = BtTrue;

	// Make a note of the archive name
    BtStrCopy( m_lastArchiveName, 32, m_archiveName );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlArchive::Update()
{
  	if( BtStrCompare( m_lastArchiveName, m_archiveName ) == BtFalse )
	{
		OnExit();
		Load();
		OnEntry();

		m_justLoaded = BtTrue;
	}
	else
	{
		m_justLoaded = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Unload

void HlArchive::Unload()
{
	if( m_isLoaded )
	{	
		// Clear the archive name
		BtStrCopy(m_lastArchiveName, "");

		// Unload the archive
		m_archive.Unload();

		// Unload the archive
		m_isLoaded = BtFalse;
	}	
}

////////////////////////////////////////////////////////////////////////////////
// PaFileDetails.h

// Include guard
#pragma once
#include "PaFilename.h"
#include "BtTypes.h"

// Holds information on a found file
class PaFileDetails
{
public:

	PaFileDetails();
	PaFileDetails( const BtChar* pPath, const BtChar* pName );

	BtChar			m_szName[MaxFilePath];						// "test.bmp"
	BtChar			m_szTitle[MaxFilePath];						// "test"
    BtChar			m_szPath[MaxFilePath];						// "c:\resources\pictures"
	BtChar			m_szFilename[MaxFilePath];					// "c:\resources\pictures\test.bmp"
	BtChar			m_szExtension[MaxFilePath];					// "bmp"
	BtChar			m_szFolder[MaxFilePath];					// "pictures"
	BtBool			m_bFolder;									// Folder (not a file, used while recursing)
	BtU64			m_tResourceChanged;							// Timestamp of the last file change
};

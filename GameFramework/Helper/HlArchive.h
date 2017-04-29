////////////////////////////////////////////////////////////////////////////////
// HlArchive.h

// Include guard
#pragma once

// Includes
#include "BaArchive.h"
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"

// Class definition
class HlArchive
{
public:
	
	// Public functions
	void									Load();

	virtual void							Update() = 0;
	virtual void							Render() = 0;

	virtual void							OnExit() = 0;
	virtual void							OnEntry() = 0;

	void									RenderOVR();
	void									Unload();

	// Accessors

protected:

	BtChar									m_archiveName[32];
	BtChar									m_lastArchiveName[32];

	BaArchive								m_archive;
	BtBool									m_isLoaded = BtFalse;
	BtBool									m_justLoaded = BtFalse;
};

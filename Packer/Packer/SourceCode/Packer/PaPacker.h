////////////////////////////////////////////////////////////////////////////////
// PaPacker

#ifndef __PaPacker_h__
#define __PaPacker_h__

#include "BtTypes.h"
#include "BtSingleton.h"
#include "BaArchive.h"
#include "FsFindFile.h"
#include "ExSchedule.h"
#include "PaPackerInterface.h"
#include "PaMemoryResource.h"
#include "PaArchive.h"
#include <vector>

// Typedef the external function
typedef BtBool (*DLLPackerProc)( PaPackerInterface& exportDetails );

class PaPacker : public BtSingleton< PaPacker >
{
public:

	PaPacker();

	void							Pack();

	DLLPackerProc					PackerProc();
	void							PackerProc( DLLPackerProc PackerProc );

	static void						GlobalID( BtU32 nGlobalID );
	static BtU32					GlobalID();
	static void						AdvanceGlobalID();

private:

	// Private functions
	BtChar						   *GetResourceBuildListName();
	BtChar						   *GetArchiveBuildListName();
	BtChar						   *GetAssetBuildListName();

	PaAsset*						GetAsset( BtChar* pFilename );
	BtBool							isDependency( BtChar* pFilename );
	void							FindArchivesAndFiles();
	void							BuildAssetList();

	BtU64							GetFileTime();

	void							LoadGlobalID();
	void							LoadArchiveBuildTimes();
	void							ReadAssetBuildTimes();
	void							ReadResourceList();
	void							FlagForExport();
	void							CompileResources();
	void							SetDataOffsets();
    void							LoadResourceHeaders();
	void							PackResources();
	void							DeflateArchive();
	void							WriteArchiveBuildTimes();
	void							WriteAssetBuildTimes();
	void							WriteResourceList();
	void							WriteGlobalID();
	void							DeleteArchives();

	// Members
	std::vector<PaArchive*>			m_archives;
	BtU32							m_archiveCount;
	BtU32							m_nResources;
	BtU32							m_nFiles;
	BtU32							m_nToExport;

	DLLPackerProc					m_pPackerProc;

	PaArchive*						m_pArchive;							// Current archive

	static BtU32					m_nGlobalID;

	BtBool							m_hasExported;

	// 
	BtChar							m_resourceListName[256];
	BtChar							m_archiveListName[256];
};

////////////////////////////////////////////////////////////////////////////////
// GlobalID

inline void PaPacker::GlobalID( BtU32 nGlobalID )
{
	m_nGlobalID = nGlobalID;
}

////////////////////////////////////////////////////////////////////////////////
// GlobalID

inline BtU32 PaPacker::GlobalID()
{
	return m_nGlobalID;
}

////////////////////////////////////////////////////////////////////////////////
// AdvanceGlobalID

inline void PaPacker::AdvanceGlobalID()
{
	++m_nGlobalID;
}

////////////////////////////////////////////////////////////////////////////////
// PackerProc

inline DLLPackerProc PaPacker::PackerProc()
{
	return m_pPackerProc;
}

////////////////////////////////////////////////////////////////////////////////
// PackerProc

inline void PaPacker::PackerProc( DLLPackerProc PackerProc )
{
	m_pPackerProc = PackerProc;
}

#endif // __PaPacker_h__

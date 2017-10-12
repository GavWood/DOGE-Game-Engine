////////////////////////////////////////////////////////////////////////////////
// ExResource

#ifndef __ExResource_h__
#define __ExResource_h__

#include "PaAsset.h"
#include "PaMemoryResource.h"
#include "BaArchive.h"
#include "BaFileData.h"

class ExResource
{
public:

	ExResource( PaFileDetails& FileDetails, PaAsset* pAsset, PaArchive* pArchive );
	ExResource();

	virtual void Create();
	virtual void Destroy();
	virtual void Export();

	BtU32 AddResource( PaMemoryResource& memoryResource,
					   const BtChar* pFilename,
					   const BtChar* pTitle,
					   BaResourceType eType );

	void AddDependency( BtChar* pFilename );

	// Accessors
	void GetFilename( const BtChar* pFilename );
	void GetTitle( const BtChar* pTitle );

	void nBaseResourceID( BtU32 nResourceID );
	BtU32 nBaseResourceID();
	BtU32 GetResourceID();
	void SetResourceID(BtU32 resourceID) { m_nResourceID = resourceID; }
	BtChar* pWorkingPath();
	BtChar* GetFilename();
	BtChar* pPath();
	BtChar* GetTitle();
	BtChar* GetExtension();
	PaAsset* pAsset();
	PaArchive* pArchive();

protected:

	BtU32									m_nResourceID;
	BtU32									m_nBaseResourceID;
	PaArchive*								m_pArchive;
	PaAsset*								m_pAsset;
	PaFileDetails							m_fileDetails;
	BtChar									m_workingPath[MaxFilePath];
};

////////////////////////////////////////////////////////////////////////////////
// Export

inline void ExResource::Export()
{}

////////////////////////////////////////////////////////////////////////////////
// pFilename

inline void ExResource::GetFilename( const BtChar* pFilename )
{
	BtStrCopy( m_fileDetails.m_szFilename, MaxFilePath, pFilename );
}

////////////////////////////////////////////////////////////////////////////////
// pTitle

inline void ExResource::GetTitle( const BtChar* pTitle )
{
	BtStrCopy( m_fileDetails.m_szTitle, MaxFilePath, pTitle );
}

////////////////////////////////////////////////////////////////////////////////
// pArchive

inline PaArchive* ExResource::pArchive()
{
	return m_pArchive;
}

////////////////////////////////////////////////////////////////////////////////
// pWorkingPath

inline BtChar* ExResource::pWorkingPath()
{
	return m_workingPath;
}

////////////////////////////////////////////////////////////////////////////////
// nResourceID

inline BtU32 ExResource::GetResourceID()
{
	return m_nResourceID;
}

////////////////////////////////////////////////////////////////////////////////
// nBaseResourceID

inline void ExResource::nBaseResourceID( BtU32 nResourceID )
{
	m_nBaseResourceID = nResourceID;
}

////////////////////////////////////////////////////////////////////////////////
// nBaseResourceID

inline BtU32 ExResource::nBaseResourceID()
{
	return m_nBaseResourceID;
}

////////////////////////////////////////////////////////////////////////////////
// pFilename

inline BtChar* ExResource::GetFilename()
{
	return m_fileDetails.m_szFilename;
}

////////////////////////////////////////////////////////////////////////////////
// pTitle

inline BtChar* ExResource::GetTitle()
{
	return m_fileDetails.m_szTitle;
}

////////////////////////////////////////////////////////////////////////////////
// pPath

inline BtChar* ExResource::pPath()
{
	return m_fileDetails.m_szPath;
}

////////////////////////////////////////////////////////////////////////////////
// pExtension

inline BtChar* ExResource::GetExtension()
{
	return m_fileDetails.m_szExtension;
}

#endif // __ExResource_h__

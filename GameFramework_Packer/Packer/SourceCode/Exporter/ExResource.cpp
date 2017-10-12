////////////////////////////////////////////////////////////////////////////////
// ExResource

#include "BtTypes.h"
#include "BtString.h"
#include "PaArchive.h"
#include "PaTopState.h"
#include "ExResource.h"
#include "ExExporter.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExResource::ExResource()
{
	m_pAsset = ExExporter::Instance().pCurrentAsset();
	m_pArchive = m_pAsset->pArchive();
	BtMemory::Copy( &m_fileDetails, &m_pAsset->File(), sizeof(PaFileDetails) );
	m_nResourceID = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Create

void ExResource::Create()
{

}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void ExResource::Destroy()
{

}

////////////////////////////////////////////////////////////////////////////////
// AddResource

BtU32 ExResource::AddResource( PaMemoryResource& memoryResource,
							   const BtChar* pFilename,
							   const BtChar* pTitle,
							   BaResourceType eType )
{
	m_nResourceID = m_pArchive->AddResource( m_pAsset,
											 memoryResource,
											 pFilename,
											 pTitle,
											 eType );
	return m_nResourceID;
}

void ExResource::AddDependency( BtChar* pFilename )
{
	m_pAsset->AddDependency( pFilename );
}

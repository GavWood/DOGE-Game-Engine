////////////////////////////////////////////////////////////////////////////////
// ExMaterialList.cpp

#include "PaMemoryResource.h"
#include "PaArchive.h"
#include "ExTexture.h"
#include "ExMaterialList.h"

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExMaterialList::~ExMaterialList()
{
	BtU32 nMaterialCount = (BtU32) m_pMaterials.size();

	for( BtU32 nMaterial=0; nMaterial<nMaterialCount; nMaterial++ )
	{
		// Cache each material
		ExMaterial* pMaterial = m_pMaterials[nMaterial];
		
		delete pMaterial;
	}
}

////////////////////////////////////////////////////////////////////////////////
// pFind

ExMaterial* ExMaterialList::pFind( ExMaterial* pToFindMaterial )
{
	BtU32 nMaterialCount = (BtU32) m_pMaterials.size();

	for( BtU32 nMaterial=0; nMaterial<nMaterialCount; nMaterial++ )
	{
		// Cache each material
		ExMaterial* pMaterial = m_pMaterials[nMaterial];

		(void) pMaterial;
		(void) pToFindMaterial;
	}
	return pToFindMaterial;
}

////////////////////////////////////////////////////////////////////////////////
// pMaterial

ExMaterial* ExMaterialList::pMaterial( BtU32 nMaterial )
{
	return m_pMaterials[nMaterial];
}

////////////////////////////////////////////////////////////////////////////////
// Add

void ExMaterialList::Add( ExMaterial* pMaterial )
{
	m_pMaterials.push_back( pMaterial );
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExMaterialList::Export()
{
	BtU32 numMaterials = GetNumMaterials();

	// Loop through the materials
	for( BtU32 nMaterial=0; nMaterial<numMaterials; nMaterial++ )
	{
		// Cache each material pointer
		ExMaterial* pCurrentMaterial = pMaterial( nMaterial );

		// Export the material
		pCurrentMaterial->Export();
	}
}


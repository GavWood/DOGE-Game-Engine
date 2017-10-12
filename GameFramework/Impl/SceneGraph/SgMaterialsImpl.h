////////////////////////////////////////////////////////////////////////////////
// SgMaterialsWinGL.h

#pragma once
#include "BtTypes.h"
#include "BaFileData.h"
#include "SgMaterial.h"
#include "ErrorLog.h"
#include "RsMaterialImpl.h"

class RsMaterial;
class SgNode;

// Class Declaration
class SgMaterialsWinGL : public SgMaterial
{
public:

	RsMaterial*					GetMaterial( BtU32 iIndex );
	void						SetMaterial( BtU32 iMaterial, RsMaterial* pMaterial );
	BtU32						NumMaterials() const;
	void						FixPointers( BaArchive *pArchive, BtU8* pMemory );
	void						Render();

private:					
	friend class SgMeshWin32;
	friend class SgSkinWin32;
	friend class SgNodeWinDX;
	friend class SgNodeImpl;

	SgNode					   *m_pNode;
	BaMaterialsFileData*		m_pMaterialsFileData;
	BaMaterialBlockFileData*	m_pMaterialBlocks;
};

////////////////////////////////////////////////////////////////////////////////
// GetMaterial

inline RsMaterial* SgMaterialsWinGL::GetMaterial( BtU32 iMaterial )
{
#ifdef _DEBUG
	if( iMaterial >= m_pMaterialsFileData->m_nMaterials )
	{
		ErrorLog::Fatal_Printf( "SgMaterials: GetMaterial. Index %d out of bounds.\r\n", iMaterial );
	}
#endif
	return m_pMaterialBlocks[iMaterial].m_pMaterial;
}

////////////////////////////////////////////////////////////////////////////////
// NumMaterials

inline BtU32 SgMaterialsWinGL::NumMaterials() const
{
	return m_pMaterialsFileData->m_nMaterials;
}

////////////////////////////////////////////////////////////////////////////////
// SetMaterial

inline void SgMaterialsWinGL::SetMaterial( BtU32 iMaterial, RsMaterial* pMaterial )
{
	if( iMaterial >= m_pMaterialsFileData->m_nMaterials )
	{
		ErrorLog::Fatal_Printf( "SgMaterials: SetMaterial. Index %d out of bounds.\r\n", iMaterial );
	}
	
	RsMaterialImpl* pMaterialWin32 = (RsMaterialImpl*) pMaterial;
	
	RsMaterialImpl* pMaterialBlockWin32 = (RsMaterialImpl*) m_pMaterialBlocks[iMaterial].m_pMaterial;

	pMaterialWin32->m_pFileData->m_vertexType = pMaterialBlockWin32->GetVertexType();

//	if( pMaterialWin32->GetVertexType() != m_pMaterialBlocks[iMaterial].m_pMaterial->GetVertexType() )
//	{
//		ErrorLog::Fatal_Printf( "SgMaterials: SetMaterial. Index %d out of bounds.\r\n", iMaterial );	
//	}

	m_pMaterialBlocks[iMaterial].m_pMaterial = pMaterial;
}

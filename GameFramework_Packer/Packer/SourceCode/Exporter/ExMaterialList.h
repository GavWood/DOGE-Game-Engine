////////////////////////////////////////////////////////////////////////////////
// ExMaterialList

#ifndef __ExMaterialList_h__
#define __ExMaterialList_h__

#include "BaArchive.h"
#include "BtTypes.h"
#include "ExMaterial.h"
#include <vector>

struct ExMaterialList
{
public:

	~ExMaterialList();

	ExMaterial*						pFind( ExMaterial* pMaterial );
	ExMaterial*						pMaterial( BtU32 nMaterial );
	void							Add( ExMaterial* pMaterial );
	void							Export();
	BtU32							GetNumMaterials();

private:

	std::vector<ExMaterial*>		m_pMaterials;
};

////////////////////////////////////////////////////////////////////////////////
// GetNumMaterials

inline BtU32 ExMaterialList::GetNumMaterials()
{
	return (BtU32) m_pMaterials.size();
}

#endif // __ExMaterialList_h__

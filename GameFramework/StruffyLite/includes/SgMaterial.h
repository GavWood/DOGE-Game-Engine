////////////////////////////////////////////////////////////////////////////////
// SgMaterial.h

#pragma once
#include "BtTypes.h"

class RsMaterial;

// Class Declaration
class SgMaterial
{
public:

	virtual RsMaterial*				GetMaterial( BtU32 iIndex ) = 0;
	virtual void					SetMaterial( BtU32 iIndex, RsMaterial* pMaterial ) = 0;
	virtual BtU32					NumMaterials() const = 0;
};

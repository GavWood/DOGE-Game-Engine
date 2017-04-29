////////////////////////////////////////////////////////////////////////////////
// SgMesh.h

#pragma once
#include "BtTypes.h"
#include "MtAABB.h"
#include "MtSphere.h"

// Class Declaration
class SgMesh
{
public:
	
	// Accessors
	virtual const MtAABB&		GetAABB() const = 0;
	virtual const MtSphere&		GetSphere() const = 0;
	virtual const BtU32			GetNumMaterials() const = 0;
};

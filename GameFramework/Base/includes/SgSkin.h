////////////////////////////////////////////////////////////////////////////////
// SgSkin.h

#pragma once
#include "BtTypes.h"
#include "MtAABB.h"
#include "MtSphere.h"

const BtU32 MaxSkeleton = 1024;
// Class Declaration
class SgSkin
{
public:
	
	// Accessors
	virtual const MtAABB& AABB() const = 0;
	virtual const MtSphere& Sphere() const = 0;
};

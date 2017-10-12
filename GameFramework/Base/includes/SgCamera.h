////////////////////////////////////////////////////////////////////////////////
// SgCamera.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"

// Class Declaration
class SgCamera
{
public:
	
	// Accessors

	virtual BtFloat FarPlane() const = 0;
	virtual BtFloat NearPlane() const = 0;
	virtual BtFloat FieldOfView() const = 0;
};

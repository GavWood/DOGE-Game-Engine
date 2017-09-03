////////////////////////////////////////////////////////////////////////////////
// SgBone.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "MtMatrix4.h"

// Class Declaration
class SgBone
{
public:
	
	// Accessors
	virtual const MtMatrix4&			GetInverseBindPose() const = 0;

//private:
	MtMatrix4							m_boneTransform;
};

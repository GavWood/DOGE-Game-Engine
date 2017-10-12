////////////////////////////////////////////////////////////////////////////////
// SgRigidBody.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"

// Class Declaration
class SgRigidBody
{
public:
	
	virtual MtVector3		CenterOfMass() = 0;
	virtual BtFloat			GetMass() = 0;
	virtual void			SetMass( BtFloat mass ) = 0;
	virtual BtBool			IsDynamic() = 0;
	virtual BtFloat			Restitution() = 0;
	virtual BtFloat			Friction() = 0;
};



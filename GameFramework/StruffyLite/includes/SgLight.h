////////////////////////////////////////////////////////////////////////////////
// SgLight.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"

enum RsLightType
{
	RsLT_Point		= 1 << 0,
	RsLT_Cone		= 1 << 1,
	RsLT_Direction  = 1 << 2,
	RsLT_Ambient	= 1 << 3,
	RsLT_Max		= 1 << 4
};

// Class Declaration
class SgLight
{
public:
	
	// Accessors
	virtual BtBool				IsCone() const = 0;
	virtual BtBool				IsPoint() const = 0;
	virtual BtBool				IsDirectional() const = 0;
	virtual RsColour&			GetColour() const = 0;
	virtual void				SetColour( const RsColour& colour ) = 0;
	virtual SgNode*				pNode() const = 0;
	virtual BtBool				GetEnabled() const = 0;
	virtual void				SetEnabled( BtBool enabled ) = 0;
	virtual BtFloat				GetInnerAngle() const = 0;
	virtual BtFloat				GetIntensity() const = 0;
	virtual void				SetIntensity( BtFloat intensity ) = 0;
};

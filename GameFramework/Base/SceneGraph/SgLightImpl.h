////////////////////////////////////////////////////////////////////////////////
// SgLightImpl.h

#pragma once
#include "SgLight.h"
#include "ErrorLog.h"

struct BaSgLightFileData;

// Class Declaration
class SgLightImpl : public SgLight
{
public:

	SgLightImpl();
	
	void						FixPointers( BtU8* pMemory );

	// Accessors
	SgNode*						pNode() const;
	RsColour&					GetColour() const;
	void						SetColour( const RsColour& colour );
	BtBool						GetEnabled() const;
	BtFloat						GetInnerAngle() const;
	void						SetEnabled( BtBool enabled );
	BtFloat						GetIntensity() const;
	void						SetIntensity( BtFloat intensity );
	BtBool						IsCone() const;
	BtBool						IsPoint() const;
	BtBool						IsDirectional() const;

private:

	friend class SgNodeDX11;
	friend class SgNodeImpl;

	SgNode					   *m_pNode;
	BaSgLightFileData*			m_pFileData;
};

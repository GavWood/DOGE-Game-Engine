////////////////////////////////////////////////////////////////////////////////
// SgLightDX11.h

#pragma once
#include "SgLight.h"
#include "ErrorLog.h"

struct BaSgLightFileData;

// Class Declaration
class SgLightDX11 : public SgLight
{
public:

	SgLightDX11();
	
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
	friend class SgNodeWinGL;

	SgNode					   *m_pNode;
	BaSgLightFileData*			m_pFileData;
};

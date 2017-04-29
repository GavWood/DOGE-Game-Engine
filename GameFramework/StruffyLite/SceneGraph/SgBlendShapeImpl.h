////////////////////////////////////////////////////////////////////////////////
// SgBlendShapeDX11.h

#pragma once
#include "SgBlendShape.h"
#include "BaFileData.h"
#include "ErrorLog.h"

// Class Declaration
class SgBlendShapeImpl : public SgBlendShape
{
public:

	void						FixPointers( BtU8* pMemory );
	BtU32						Items();
	void						Weight( BtU32 iIndex, BtFloat weight );
	BtFloat						Weight( BtU32 iIndex );
	BtFloat*					pWeights() const;

private:

	friend class SgNodeDX11;
	friend class SgNodeWinGL;
	friend class SgAnimatorWinGL;

	SgNode					   *m_pNode;
	BaSgBlendShapeFileData*		m_pFileData;
};

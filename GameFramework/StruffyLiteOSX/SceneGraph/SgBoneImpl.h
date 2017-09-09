////////////////////////////////////////////////////////////////////////////////
// SgBoneGLES.h

#pragma once

#include "SgBone.h"
#include "BaFileData.h"
#include "ErrorLog.h"

class SgNodeWinGL;

// Class Declaration
class SgBoneOSX : public SgBone
{
public:

	void FixPointers( BtU8* pMemory );
	const MtMatrix4& GetInverseBindPose() const;

private:

	friend class SgNodeWinGL;
	friend class SgAnimatorWinGL;

	SgNodeWinGL* m_pNode;
	BaSgBoneFileData* m_pFileData;
};

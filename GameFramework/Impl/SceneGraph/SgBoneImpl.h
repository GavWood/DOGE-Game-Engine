////////////////////////////////////////////////////////////////////////////////
// SgBoneImpl.h

#pragma once

#include "SgBone.h"
#include "BaFileData.h"
#include "ErrorLog.h"

class SgNodeImpl;

// Class Declaration
class SgBoneImpl : public SgBone
{
public:

	void FixPointers( BtU8* pMemory );
	const MtMatrix4& GetInverseBindPose() const;

private:

	friend class SgNodeImpl;
	friend class SgAnimatorImpl;

	SgNodeImpl* m_pNode;
	BaSgBoneFileData* m_pFileData;
};

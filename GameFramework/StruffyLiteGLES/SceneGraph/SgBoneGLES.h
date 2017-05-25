////////////////////////////////////////////////////////////////////////////////
// SgBoneGLESß.h

#pragma once

#include "SgBone.h"
#include "BaFileData.h"
#include "ErrorLog.h"

class SgNodeWinDX;

// Class Declaration
class SgBoneWin32 : public SgBone
{
public:

	void FixPointers( BtU8* pMemory );
	const MtMatrix4& InverseBindPose() const;

private:

	friend class SgNodeWinGL;
	friend class SgAnimatorWinGL;

	SgNodeWinGL* m_pNode;
	BaSgBoneFileData* m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// FixPointers

inline void SgBoneWin32::FixPointers( BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgBoneFileData*) pMemory;
}

////////////////////////////////////////////////////////////////////////////////
// InverseBindPose

inline const MtMatrix4& SgBoneWin32::InverseBindPose() const
{
	return m_pFileData->m_m4InverseBindPose;
}

////////////////////////////////////////////////////////////////////////////////
// SgBoneDX11.h

#pragma once
#include "SgBone.h"
#include "BaFileData.h"
#include "ErrorLog.h"

// Class Declaration
class SgBoneDX11 : public SgBone
{
public:

	void							FixPointers( BtU8* pMemory );
	const MtMatrix4&				GetInverseBindPose() const;

private:

	friend class SgNodeDX11;
	friend class SgNodeWinDX;
	friend class SgNodeImpl;
	friend class SgAnimatorImpl;

	SgNode						   *m_pNode;
	BaSgBoneFileData*				m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// FixPointers

inline void SgBoneDX11::FixPointers( BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgBoneFileData*) pMemory;
}

////////////////////////////////////////////////////////////////////////////////
// GetInverseBindPose

inline const MtMatrix4& SgBoneDX11::GetInverseBindPose() const
{
	return m_pFileData->m_m4InverseBindPose;
}

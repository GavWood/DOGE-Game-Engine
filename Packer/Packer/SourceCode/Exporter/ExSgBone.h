////////////////////////////////////////////////////////////////////////////////
// ExSgBone.h

#ifndef __ExSgBone_h__
#define __ExSgBone_h__

#include "MtAABB.h"
#include "BaArchive.h"
#include <vector>

class ExScene;
class ExMaterialBlock;
struct ExSgNode;

class ExSgBone
{
public:

	ExSgBone( ExSgNode* pNode, ExScene *pScene, BtU32 iJoint );
	~ExSgBone();

	void						CopyAttributes();
	void						ChangeCoordinateSystem();

	// Accessors
	void						InverseBindPose( const MtMatrix4& m4InverseBindPose );
	const MtMatrix4&			InverseBindPose() const;
	BtU32						BoneIndex() const;
	void						BoneIndex( BtU32 iBone );

private:

	friend class ExScene;
	friend class ExCollisionBone;
	
	ExSgNode*					m_pNode;
	ExScene*					m_pScene;
	BtU32						m_iBone;
	BtU32						m_iJoint;
	MtMatrix4 					m_m4InverseBindPose;
};

///////////////////////////////////////////////////////////////////////////////
// BoneIndex

inline void ExSgBone::BoneIndex( BtU32 iBone )
{
	m_iBone = iBone;
}

////////////////////////////////////////////////////////////////////////////////
// InverseBindPose

inline const MtMatrix4& ExSgBone::InverseBindPose() const
{
	return m_m4InverseBindPose;
}

inline void ExSgBone::InverseBindPose( const MtMatrix4& m4InverseBindPose )
{
	m_m4InverseBindPose = m4InverseBindPose;
}

////////////////////////////////////////////////////////////////////////////////
// BoneIndex

inline BtU32 ExSgBone::BoneIndex() const
{
	return m_iBone;
}

#endif // __ExSgBone_h__

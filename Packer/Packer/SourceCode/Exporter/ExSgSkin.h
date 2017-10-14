////////////////////////////////////////////////////////////////////////////////
// ExSgSkin.h

#ifndef __ExSgSkin_h__
#define __ExSgSkin_h__

#include "MtAABB.h"
#include "MtSphere.h"
#include "BaArchive.h"
#include <vector>

const BtU32 MaxBones = 256;

class ExScene;
class ExMaterialBlock;
struct ExSgNode;

class ExSgSkin
{
public:

	ExSgSkin( ExSgNode* pNode, ExScene *pScene );
	~ExSgSkin();

	void							CopyAttributes();
	void							ChangeCoordinateSystem();

	// Accessors
	const MtAABB&					AABB() const;
	const MtSphere&					Sphere() const;

	//BtU32							BoneMap( BtU32 iJoint );

private:

	friend class ExScene;
	friend class ExCollisionSkin;

	void							GroupDrawing();
	void							OptimiseGeometry();
	void							BoundVertex();
	void							MakeBoneGroups();
	void							MoveToVertexBuffers();

	MtAABB							m_AABB;
	MtSphere						m_sphere;
	MtVector3						m_v3Center;
	ExSgNode*						m_pNode;
	ExScene*						m_pScene;
	std::vector<ExMaterialBlock*>	m_materialBlocks;
	BtU32							m_boneMap[MaxBones];

	ExSgNode*						m_skeleton[64];
	BtU32							m_numJoints;
};

/*
////////////////////////////////////////////////////////////////////////////////
// BoneMap

inline BtU32 ExSgSkin::BoneMap( BtU32 iJoint )
{
	return m_boneMap[iJoint];
}
*/

////////////////////////////////////////////////////////////////////////////////
// AABB

inline const MtAABB& ExSgSkin::AABB() const
{
	return m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
// Sphere

inline const MtSphere& ExSgSkin::Sphere() const
{
	return m_sphere;
}

#endif // __ExSgSkin_h__

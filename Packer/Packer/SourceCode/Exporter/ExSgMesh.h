////////////////////////////////////////////////////////////////////////////////
// ExSgMesh.h

#pragma once
#include "MtAABB.h"
#include "MtSphere.h"
#include "BaArchive.h"
#include <vector>

class ExScene;
class ExTexture;
class ExMaterialBlock;
class ExVertexBuffer;
struct ExSgNode;

class ExSgMesh
{
public:

	ExSgMesh( ExSgNode* pNode, ExScene *pScene );
	~ExSgMesh();

	void							CopyAttributes();
	void							ChangeCoordinateSystem();
	void							MoveToVertexBuffers();

	// Accessors
	const MtAABB&					AABB() const;
	const MtSphere&					Sphere() const;

	// Materials
	BtU32							NumMaterialBlocks();
	ExMaterialBlock*				pMaterialBlock( BtU32 iMaterialBlock );

private:

	friend class ExScene;
	friend class ExSgCollision;

	void							OptimiseGeometry();
	void							GroupDrawing();
	void							BoundVertex();
	void							MakeBoneGroups();
	void							MakeRenderGroups();

	MtAABB							m_AABB;
	MtSphere						m_sphere;
	MtVector3						m_v3Center;
	BtBool							m_hasDimensions;
	ExSgNode*						m_pNode;
	ExScene*						m_pScene;
	std::vector<ExMaterialBlock*>	m_materialBlocks;
};

////////////////////////////////////////////////////////////////////////////////
// AABB

inline const MtAABB& ExSgMesh::AABB() const
{
	return m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
// Sphere

inline const MtSphere& ExSgMesh::Sphere() const
{
	return m_sphere;
}

////////////////////////////////////////////////////////////////////////////////
// NumMaterialBlocks

inline BtU32 ExSgMesh::NumMaterialBlocks()
{
	return (BtU32) m_materialBlocks.size();
}

////////////////////////////////////////////////////////////////////////////////
// pMaterialBlock

inline ExMaterialBlock* ExSgMesh::pMaterialBlock( BtU32 iMaterialBlock )
{
	return m_materialBlocks[iMaterialBlock];
}

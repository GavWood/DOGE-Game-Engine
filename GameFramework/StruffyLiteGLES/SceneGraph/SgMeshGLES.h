////////////////////////////////////////////////////////////////////////////////
/// SgMeshWin32GL.h

#ifndef __SgMeshWin32GL_h__
#define __SgMeshWin32GL_h__

#include "SgMesh.h"
#include "SgNodeGLES.h"
#include "SgRigidBodyImpl.h"
#include "BaFileData.h"
#include "ErrorLog.h"

struct SgMeshRenderable;
class RsShaderD3DEffectWin32;
class ExTexture;

// Class Declaration
class SgMeshWinGL : public SgMesh
{
public:

	void								Render();
	void								Render( SgMeshRenderable *pNodeRenderable );

	// Accessors
	const MtAABB&						GetAABB() const;
	const MtSphere&						GetSphere() const;
	const BtU32							GetNumMaterials() const;

private:

	friend class SgNodeWinGL;

	void								FixPointers( BaArchive *pArchive, BtU8* pMemory );

	SgNodeWinGL*						m_pNode;
	BaSgMeshFileData*					m_pFileData;
	ExTexture*							m_pLightmap;
};

////////////////////////////////////////////////////////////////////////////////
// AABB

inline const MtAABB& SgMeshWinGL::GetAABB() const
{
	return m_pFileData->m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
// Sphere

inline const MtSphere& SgMeshWinGL::GetSphere() const
{
	return m_pFileData->m_sphere;
}

inline const BtU32 SgMeshWinGL::GetNumMaterials() const
{
	return m_pFileData->m_nMaterials;
}

#endif // __SgMeshWin32GL_h__

////////////////////////////////////////////////////////////////////////////////
/// SgSkinWin32GL.h

#pragma once

#include "SgNodeWinGL.h"
#include "SgSkin.h"
#include "BaFileData.h"
#include "ErrorLog.h"

#include "SgRigidBodyImpl.h"

class SgBoneWinGL;
struct SgSkinRenderable;

// Class Declaration
class SgSkinWinGL : public SgSkin
{
public:

	void						Render();
	void						Render( SgSkinRenderable *pRenderable );

	// Accessors
	const MtAABB&				AABB() const;
	const MtSphere&				Sphere() const;

private:

	friend class SgNodeWinGL;
	friend class SgAnimatorWinGL;

	void						FixPointers( BaArchive *pArchive, BtU8* pMemory );

	SgNodeWinGL*				m_pNode;
	BaSgSkinFileData*			m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// AABB

inline const MtAABB& SgSkinWinGL::AABB() const
{
	return m_pFileData->m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
// Sphere

inline const MtSphere& SgSkinWinGL::Sphere() const
{
	return m_pFileData->m_sphere;
}


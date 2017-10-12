////////////////////////////////////////////////////////////////////////////////
/// SgSkinWin32GL.h

#pragma once

#include "SgNodeImpl.h"
#include "SgSkin.h"
#include "BaFileData.h"
#include "ErrorLog.h"

#include "SgRigidBodyImpl.h"

class SgBoneWinGL;
struct SgSkinRenderable;

// Class Declaration
class SgSkinImpl : public SgSkin
{
public:

	void						Render();
	void						Render( SgSkinRenderable *pRenderable );

	// Accessors
	const MtAABB&				AABB() const;
	const MtSphere&				Sphere() const;

private:

	friend class SgNodeImpl;
	friend class SgAnimatorImpl;

	void						FixPointers( BaArchive *pArchive, BtU8* pMemory );

	SgNodeImpl*				m_pNode;
	BaSgSkinFileData*			m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// AABB

inline const MtAABB& SgSkinImpl::AABB() const
{
	return m_pFileData->m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
// Sphere

inline const MtSphere& SgSkinImpl::Sphere() const
{
	return m_pFileData->m_sphere;
}


////////////////////////////////////////////////////////////////////////////////
/// SgSkinWin32GL.h

#ifndef __SgSkinWin32GL_h__
#define __SgSkinWin32GL_h__

#include "SgNodeGLES.h"
#include "SgSkin.h"
#include "BaFileData.h"
#include "ErrorLog.h"

#include "SgRigidBodyImpl.h"

class SgBoneDX11;
class SgSkinRenderable;

// Class Declaration
class SgSkinWin32GL : public SgSkin
{
public:

	void						Render();
	void						DeferredRender( SgSkinRenderable* pRenderable );

	// Accessors
	const MtAABB&				AABB() const;
	const MtSphere&				Sphere() const;

private:

	friend class SgNodeWinGL;
    friend class SgSkinWinGL;
    friend class SgAnimatorWinGL;
    
	void						FixPointers( BaArchive *pArchive, BtU8* pMemory );

	SgNodeWinGL*				m_pNode;
	BaSgSkinFileData*			m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// AABB

inline const MtAABB& SgSkinWin32GL::AABB() const
{
	return m_pFileData->m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
// Sphere

inline const MtSphere& SgSkinWin32GL::Sphere() const
{
	return m_pFileData->m_sphere;
}

#endif // __SgSkinWin32GL_h__

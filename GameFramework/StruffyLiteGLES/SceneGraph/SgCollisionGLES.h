////////////////////////////////////////////////////////////////////////////////
/// SgCollisionWin32.h

#ifndef __SgCollisionWin32_h__
#define __SgCollisionWin32_h__

#include "SgCollision.h"
#include "SgRigidBodyImpl.h"
#include "BaFileData.h"
#include "ErrorLog.h"

struct SgMeshRenderable;

class DyCollisionMesh;
class DyCollisionAnalytical;
class SgNodeWinGL;

// Class Declaration
class SgCollisionWinGL : public SgCollision
{
public:

	SgCollisionWinGL();
	
	BtU32 						NumShapes() const;
	const SgCollisionShape*		pShape( BtU32 iShape ) const;
	BtFloat*					pVertex() const;
	BtU32*						pIndex() const;

private:					

	friend class SgNodeWinDX;
	friend class SgNodeWinGL;

	void						FixPointers( BtU8* pMemory );

	SgNodeWinGL				   *m_pNode;
	DyCollisionMesh*			m_pCollisionMesh;
	DyCollisionAnalytical*		m_pCollisionAnalytical;
	BaSgCollisionFileData*		m_pFileData;
};

#endif // __SgCollisionWin32_h__

////////////////////////////////////////////////////////////////////////////////
// __ExCollisionShapes_h__

#ifndef __ExCollisionShapes_h__
#define __ExCollisionShapes_h__

#include "BtTypes.h"
#include "ExResource.h"
#include "SgCollision.h"

class ExCollisionAnalytical : public ExResource
{
public:

	ExCollisionAnalytical();
	~ExCollisionAnalytical();

	// Add each collision shape type
	void							AddBox( const MtMatrix4& transform, BtFloat x, BtFloat y, BtFloat z );
	
	void							AddCylinder( const MtMatrix4& m4Transform, BtFloat length, BtFloat radius);

	void							AddSphere( const MtMatrix4& m4Transform, BtFloat radius );

	void							AddMesh( BtChar *name,
											 const MtMatrix4& m4Transform,
											 BtU32 startIndex, BtU32 endIndex,
											 BtU32 startVertex, BtU32 endVertex );

	void							AddConvexHull(
											 BtChar *name,
											 const MtMatrix4& m4Transform,
											 BtU32 startIndex, BtU32 endIndex,
											 BtU32 startVertex, BtU32 endVertex );

	void							SerialiseBox( SgCollisionBox* pBox );
	void							SerialiseMesh( SgCollisionMesh* pMesh );
	void							SerialiseSphere( SgCollisionSphere* pSphere );
	void							SerialiseCylinder( SgCollisionCylinder* pCylinder );
	void							SerialiseConvexHull( SgCollisionHull* pHull );
	
	void							SerialisePlane();
	void							SerialiseCylinder();
	void							SerialiseCapsule();
	
	void							Export();

	BtU32							NumShapes() const;

private:

	std::vector<SgCollisionShape*>	m_collision;
	BtChar							m_name[32];
	PaMemoryResource				m_resource;
};

////////////////////////////////////////////////////////////////////////////////
// NumShapes

inline BtU32 ExCollisionAnalytical::NumShapes() const
{
	return (BtU32) m_collision.size();
}

#endif // __ExCollisionShapes_h__

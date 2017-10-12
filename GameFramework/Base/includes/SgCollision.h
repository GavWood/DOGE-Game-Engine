////////////////////////////////////////////////////////////////////////////////
// SgCollision.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "MtMatrix4.h"
#include "MtPlane.h"

#pragma pack(push,1)
#pragma pack(1)

enum SgCollisionType
{
	SgCT_Box,
	SgCT_Plane,
	SgCT_Sphere,
	SgCT_Cylinder,
	SgCT_Mesh,
	SgCT_ConvexHull,
	SgCT_Max,
};


struct BaSgCollisionPlane
{
	MtMatrix4						m_m4Transform;
	MtPlane							m_plane;
};

struct BaSgCollisionCylinder
{
	MtMatrix4						m_m4Transform;
	BtFloat							m_radius;
	BtFloat							m_height;
};

struct BaSgCollisionSphere
{
	MtMatrix4						m_m4Transform;
	BtFloat							m_radius;
	BtU32							m_pad;
};

struct BaSgCollisionBox
{
	MtMatrix4						m_m4Transform;
	MtVector3						m_v3Dimension;
	BtU32							m_pad;
};

struct BaSgCollisionMesh
{
	MtMatrix4						m_m4Transform;
	BtU32							m_startVertex;
	BtU32							m_endVertex;
	BtU32							m_startIndex;
	BtU32							m_endIndex;
};

////////////////////////////////////////////////////////////////////////////////
// shapeType

#define DECLARE_SHAPETYPE( _TyID ) \
SgCollisionType shapeType() const \
{ \
	return _TyID; \
} \

////////////////////////////////////////////////////////////////////////////////
// SgCollisionShape

struct SgCollisionShape
{
	virtual SgCollisionType shapeType() const
	{
		return SgCT_Max;
	}
};

////////////////////////////////////////////////////////////////////////////////
// SgCollisionCylinder

struct SgCollisionCylinder : public SgCollisionShape
{
	DECLARE_SHAPETYPE( SgCT_Cylinder );

	BaSgCollisionCylinder *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// SgCollisionBox

struct SgCollisionBox : public SgCollisionShape
{
	DECLARE_SHAPETYPE( SgCT_Box );

	BaSgCollisionBox *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// SgCollisionSphere

struct SgCollisionSphere : public SgCollisionShape
{
	DECLARE_SHAPETYPE( SgCT_Sphere );

	BaSgCollisionSphere *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// SgCollisionMesh

struct SgCollisionMesh : public SgCollisionShape
{
	DECLARE_SHAPETYPE( SgCT_Mesh );

	BaSgCollisionMesh *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// SgCollisionHull

struct SgCollisionHull : public SgCollisionShape
{
	DECLARE_SHAPETYPE( SgCT_ConvexHull );

	BaSgCollisionMesh *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// SgCollision

class SgCollision
{
public:

	virtual BtU32			NumShapes() const = 0;
	virtual const			SgCollisionShape* pShape( BtU32 shapeIndex ) const = 0;

	virtual BtFloat*		pVertex() const = 0;
	virtual BtU32*			pIndex() const = 0;
};

//#pragma options align=reset
#pragma pack(pop) 

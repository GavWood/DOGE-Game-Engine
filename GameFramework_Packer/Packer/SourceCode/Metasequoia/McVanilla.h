////////////////////////////////////////////////////////////////////////////////
// MCVanilla.h

// Include guard
#ifndef __MCVanilla_h__
#define __MCVanilla_h__

#include "BtTypes.h"
#include "MtVector3.h"
#include "MtVector2.h"
#include "RsColour.h"
#include <vector>

class FCDocument;
class FCDGeometryPolygons;
class FCDGeometryMesh;
class FCDMaterial;
class FCDSceneNode;

struct MCVertex
{
	MtVector3					m_v3Position;
	MtVector3					m_v3Normal;
	MtVector3					m_v3NewNormal;
	MtVector2					m_v2UV;
};

struct MCFace
{
	std::vector<BtU32>			m_oldIndex;
	std::vector<BtU32>			m_newIndex;
	std::vector<MtVector2>		m_v2UV;
	BtU32						m_materialID;
	MtVector3					m_v3Normal;
	BtU32						m_newVertexStart;
};

struct MCObject
{
	// Constructor
	MCObject();

	void						AddVertex( const MtVector3& v3Coord );
	void						AddVertex( const MtVector3& v3Coord, const MtVector3& v3Normal );
	void						AddFace( MCFace& face );

	BtChar						m_name[256];
	BtU32						m_depth;

	std::vector<MCVertex>		m_oldVertex;		// As they are in Metasequoia file
	std::vector<MCVertex>		m_newVertex;		// My new vertex with proper normals

	std::vector<MCFace>			m_faces;
};

struct MCMaterial
{
	MCMaterial();

	BtChar						m_name[256];
	BtChar						m_diffuseFilename[256];
	RsColour					m_colour;
};

// Class definition
class MCVanilla
{
public:

	// Public functions
	MCVanilla();

	void						Empty();

	void						AddMaterial( const MCMaterial& material );
	void						AddObject( const MCObject& object );
	void						AddPolygons( MCObject& object );
	void						CalculateFaceNormals( MCObject& object );
	void						CalculateSmoothedNormals( MCObject& object );
	void						CalculateSmoothedNormalsOld( MCObject& object );

	// Accessors
	BtU32						GetNumObjects() const;
	BtU32						GetNumMaterials() const;
	const MCObject&				GetObject( BtU32 iObject ) const;
	const MCMaterial&			GetMaterial( BtU32 iMaterial ) const;
	BtBool						GetReversedWindingOrder() const;
	
private:

	std::vector<MCMaterial>		m_materials;
	std::vector<MCObject>		m_objects;
	BtBool						m_reverseWindingOrder;
};

inline BtBool MCVanilla::GetReversedWindingOrder() const
{
	return m_reverseWindingOrder;
}

inline BtU32 MCVanilla::GetNumObjects() const
{
	return (BtU32)m_objects.size();
}

inline BtU32 MCVanilla::GetNumMaterials() const
{
	return (BtU32)m_materials.size();
}

inline const MCObject& MCVanilla::GetObject( BtU32 iObject ) const
{
	return m_objects[iObject];
}

inline const MCMaterial& MCVanilla::GetMaterial( BtU32 iMaterial ) const
{
	return m_materials[iMaterial];
}

#endif // __MCVanilla_h__

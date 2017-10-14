////////////////////////////////////////////////////////////////////////////////
// ExCollisionMesh

#include "BaArchive.h"
#include "BtCRC.h"
#include "DyCollisionAnalytical.h"
#include "ExCollisionAnalytical.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExCollisionAnalytical::ExCollisionAnalytical()
	: m_resource( PMR_Write )
{
	strcpy( m_name, "" );
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExCollisionAnalytical::~ExCollisionAnalytical()
{
	// Cache the number of shapes
	BtU32 NumShapes = (BtU32) m_collision.size();

	// Iterate through the shapes
	for( BtU32 iShape=0; iShape<NumShapes; ++iShape)
	{
		// Cache each collision entry
		SgCollisionShape* pCollisionShape = m_collision[iShape];

		switch( pCollisionShape->shapeType() )
		{
			case SgCT_Box:
				break;
			case SgCT_Plane:
				break;
			case SgCT_Sphere:
				break;
			case SgCT_Cylinder:
				break;
			case SgCT_Mesh:
				break;
			case SgCT_ConvexHull:
				break;
            case SgCT_Max:
                break;
		}

		// Delete the collision shape
		delete pCollisionShape;
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddSphere

void ExCollisionAnalytical::AddSphere( const MtMatrix4& m4Transform, BtFloat radius )
{
	SgCollisionSphere* pSphere = new SgCollisionSphere;
	pSphere->m_pFileData = new BaSgCollisionSphere;
	pSphere->m_pFileData->m_m4Transform = m4Transform;
	pSphere->m_pFileData->m_radius = radius;
	m_collision.push_back( pSphere );
}

////////////////////////////////////////////////////////////////////////////////
// AddCylinder

void ExCollisionAnalytical::AddCylinder( const MtMatrix4& m4Transform, BtFloat length, BtFloat radius)
{
	SgCollisionCylinder* pCylinder = new SgCollisionCylinder;
	pCylinder->m_pFileData = new BaSgCollisionCylinder;
	pCylinder->m_pFileData->m_m4Transform = m4Transform;
	pCylinder->m_pFileData->m_height = length;
	pCylinder->m_pFileData->m_radius = radius;
	m_collision.push_back(pCylinder);
}

////////////////////////////////////////////////////////////////////////////////
// AddBox

void ExCollisionAnalytical::AddBox( const MtMatrix4& m4Transform, BtFloat x, BtFloat y, BtFloat z)
{
	SgCollisionBox* pBox = new SgCollisionBox;
	pBox->m_pFileData = new BaSgCollisionBox;
	pBox->m_pFileData->m_m4Transform = m4Transform;
	pBox->m_pFileData->m_v3Dimension = MtVector3( x, y, z );
	m_collision.push_back(pBox);
}

////////////////////////////////////////////////////////////////////////////////
// AddMesh

void ExCollisionAnalytical::AddMesh( BtChar *name,
									 const MtMatrix4& m4Transform,
									 BtU32 startIndex, BtU32 endIndex,
									 BtU32 startVertex, BtU32 endVertex  )
{
	strcpy( m_name, name );
	SgCollisionMesh* pMesh = new SgCollisionMesh;
	pMesh->m_pFileData = new BaSgCollisionMesh;
	pMesh->m_pFileData->m_m4Transform = m4Transform;
	pMesh->m_pFileData->m_startVertex = startVertex;
	pMesh->m_pFileData->m_endVertex   = endVertex;
	pMesh->m_pFileData->m_startIndex  = startIndex;
	pMesh->m_pFileData->m_endIndex    = endIndex;
	m_collision.push_back( pMesh );
}

////////////////////////////////////////////////////////////////////////////////
// AddConvexHull

void ExCollisionAnalytical::AddConvexHull( BtChar *name,
										   const MtMatrix4& m4Transform,
										   BtU32 startIndex, BtU32 endIndex,
										   BtU32 startVertex, BtU32 endVertex  )
{
	strcpy(m_name, name);
	SgCollisionHull* pConvexHull = new SgCollisionHull;
	pConvexHull->m_pFileData = new BaSgCollisionMesh;
	pConvexHull->m_pFileData->m_m4Transform = m4Transform;
	pConvexHull->m_pFileData->m_startVertex = startVertex;
	pConvexHull->m_pFileData->m_endVertex = endVertex;
	pConvexHull->m_pFileData->m_startIndex = startIndex;
	pConvexHull->m_pFileData->m_endIndex = endIndex;
	m_collision.push_back( pConvexHull );
}

////////////////////////////////////////////////////////////////////////////////
// ExtractBox

void ExCollisionAnalytical::SerialiseBox( SgCollisionBox* pBox )
{
	m_resource << *pBox->m_pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// SerialiseMesh

void ExCollisionAnalytical::SerialiseMesh( SgCollisionMesh* pMesh )
{
	m_resource << *pMesh->m_pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// SerialiseConvexHull

void ExCollisionAnalytical::SerialiseConvexHull( SgCollisionHull* pHull )
{
	m_resource << *pHull->m_pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// SerialiseMesh

void ExCollisionAnalytical::SerialiseSphere( SgCollisionSphere* pSphere )
{
	m_resource << *pSphere->m_pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// SerialiseCylinder

void ExCollisionAnalytical::SerialiseCylinder( SgCollisionCylinder* pCylinder )
{
	m_resource << *pCylinder->m_pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExCollisionAnalytical::Export()
{
	PaMemoryResource memoryResource( PMR_Write );

	// Cache the counts of each shape type
	BtU32 numShapes = (BtU32) m_collision.size();

	// Configure the file data
	BaCollisionAnalytical fileData;
	fileData.m_numShapes = numShapes;
	fileData.m_pad = 123454321;

	// Output the file data
	m_resource << fileData;

	BtU32 shapeInstanceSize = 0;

	// Output the shapes
	for( BtU32 iShape=0; iShape<numShapes; ++iShape )
	{
		// Cache each box
		SgCollisionShape* pShape = m_collision[iShape];

		// Cache the shape type
		SgCollisionType shapeType = pShape->shapeType();

		// Export the shape type
		m_resource << shapeType;

		BtU32 tell = m_resource.Tell();
        (void)tell;
        
		// Add the box to the resource
		switch( shapeType )
		{
			case SgCT_Box :

				shapeInstanceSize += sizeof(SgCollisionBox);
				SerialiseBox( (SgCollisionBox*) pShape );
				break;

			case SgCT_Mesh :

				shapeInstanceSize += sizeof(SgCollisionMesh);
				SerialiseMesh( (SgCollisionMesh*) pShape );
				break;

			case SgCT_Sphere :

				shapeInstanceSize += sizeof(SgCollisionSphere);
				SerialiseSphere((SgCollisionSphere*)pShape);
				break;

			case SgCT_Cylinder :

				shapeInstanceSize += sizeof(SgCollisionCylinder);
				SerialiseCylinder((SgCollisionCylinder*)pShape);
				break;

			case SgCT_ConvexHull :

				shapeInstanceSize += sizeof(SgCollisionHull);
				SerialiseConvexHull( (SgCollisionHull*) pShape );
				break;
                
            case SgCT_Plane:
                break;
                
            case SgCT_Max:
                break;
		}
	}

	// Calculate the amount of room for the pointers
	//BtU32 sizePointers = sizeof(BtU64) * numShapes;

	BtU32 tell = m_resource.Tell();
    (void)tell;

	// Add the vertex resource
	BtChar shapes[32];
	sprintf( shapes, "CAnalytical_%s", m_name );
	AddResource( m_resource,
				 BtNull,
				 shapes,
				 BaRT_CollisionAnalytical );
};

////////////////////////////////////////////////////////////////////////////////
// MCVanilla.cpp

// Includes
#include <stdio.h>
#include "BtString.h"
#include "BtTypes.h"
#include "ErrorLog.h"
#include "McVanilla.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

MCObject::MCObject()
{
	BtStrEmpty( m_name, 256 );
	m_depth = 0;
	m_oldVertex.clear();
	m_newVertex.clear();
	m_faces.clear();
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

MCMaterial::MCMaterial()
{
	BtStrEmpty( m_name, 256 );
	BtStrEmpty( m_diffuseFilename, 256 );
}

////////////////////////////////////////////////////////////////////////////////
// AddVertex

void MCObject::AddVertex( const MtVector3& v3Coord )
{
	MCVertex vertex;

	vertex.m_v3Position = v3Coord;
	m_oldVertex.push_back( vertex );
}

////////////////////////////////////////////////////////////////////////////////
// AddVertex

void MCObject::AddVertex( const MtVector3& v3Coord, const MtVector3& v3Normal )
{
	MCVertex vertex;

	vertex.m_v3Position = v3Coord;
	vertex.m_v3Normal = v3Normal;
	m_oldVertex.push_back( vertex );
}

////////////////////////////////////////////////////////////////////////////////
// AddFace

void MCObject::AddFace( MCFace& face )
{
	m_faces.push_back( face );
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

MCVanilla::MCVanilla()
{
	m_reverseWindingOrder = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Empty

void MCVanilla::Empty()
{
	m_objects.clear();
	m_materials.clear();
}

////////////////////////////////////////////////////////////////////////////////
// CalculateNormals

void MCVanilla::CalculateFaceNormals( MCObject& object )
{
	ErrorLog::Printf( "CalculateFaceNormals" );

	// Get the number of faces
	BtU32 numFaces = (BtU32)object.m_faces.size();

	for( BtU32 iFace=0; iFace<numFaces; iFace++ )
	{
		// Cache each face
		MCFace& face = object.m_faces[iFace];

		// Find the normal of this face
		MtVector3 v3Normal( 0, 0, 0 );

		BtU32 indexA = face.m_oldIndex[0];
		BtU32 indexB = face.m_oldIndex[1];
		BtU32 indexC = face.m_oldIndex[2];

		MtVector3 v3A  = object.m_oldVertex[indexA].m_v3Position;
		MtVector3 v3B  = object.m_oldVertex[indexB].m_v3Position;
		MtVector3 v3C  = object.m_oldVertex[indexC].m_v3Position;

		MtVector3 v3First  = v3A - v3C;
		MtVector3 v3Second = v3B - v3C;

		// Calculate the normal
		v3Normal = v3First.CrossProduct( v3Second );

		// Reverse the windind order?
		if( m_reverseWindingOrder == BtTrue )
		{
			v3Normal = -v3Normal;
		}

		// Normalise the vector
		v3Normal.Normalise();

		// Set the normal
		face.m_v3Normal = v3Normal;
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddPolygons

void MCVanilla::AddPolygons( MCObject& object )
{
	ErrorLog::Printf( "CalculateFaceNormals" );

	// Get the number of faces
	BtU32 numFaces = (BtU32)object.m_faces.size();

	for( BtU32 iFace=0; iFace<numFaces; iFace++ )
	{
		// Cache each face
		MCFace& face = object.m_faces[iFace];

		// Cache the number of vertex on this face
		BtU32 numFaceVertex = (BtU32)face.m_oldIndex.size();

		// Set the vertex start
		face.m_newVertexStart = (BtU32)object.m_newVertex.size();

		// Add the vertex for the face
		for( BtU32 iVertexOnFace=0; iVertexOnFace<numFaceVertex; iVertexOnFace++ )
		{
			// Cache the index
			BtU32 oldIndex = face.m_oldIndex[iVertexOnFace];

			// Cache the vertex position
			MtVector3 v3Position = object.m_oldVertex[oldIndex].m_v3Position;

			// Cache the normal
			MtVector3 v3Normal = object.m_oldVertex[oldIndex].m_v3Normal;

			// Cache the vertex UV
			MtVector2 v2UV;

			if( (BtU32) face.m_v2UV.size() )
			{
				v2UV = face.m_v2UV[iVertexOnFace];
			}
			else
			{
				v2UV = MtVector2( 0, 0 );
			}

			// Set the new index
			BtU32 newIndex = (BtU32) object.m_newVertex.size();

			// Add each index
			face.m_newIndex.push_back( newIndex );

			// Make the new vertex
			MCVertex newVertex;
			newVertex.m_v3Position  = v3Position;
			newVertex.m_v3Normal    = v3Normal;
			newVertex.m_v2UV = v2UV;
			object.m_newVertex.push_back( newVertex );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// CalculateSmoothedNormals

void MCVanilla::CalculateSmoothedNormalsOld( MCObject& object )
{
	// Get the number of faces
	BtU32 numFaces = (BtU32)object.m_faces.size();

	for( BtU32 iFaceA=0; iFaceA<numFaces; iFaceA++ )
	{
		// Cache each face
		MCFace& faceA = object.m_faces[iFaceA];

		BtU32 numVertex = (BtU32)faceA.m_oldIndex.size();

		BtU32 indexCount = (BtU32)faceA.m_oldIndex.size();

		for( BtU32 i=0; i<indexCount; i++ )
		{
			// Find the average normal of the connecting faces
			MtVector3 v3AverageNormal( 0, 0, 0 );

			BtU32 indexA = faceA.m_oldIndex[i];

			for( BtU32 iFaceB=0; iFaceB<numFaces; iFaceB++ )
			{
				// Cache each face
				MCFace& faceB = object.m_faces[iFaceB];

				BtU32 oldIndexCountB = (BtU32)faceB.m_oldIndex.size();

				for( BtU32 j=0; j<oldIndexCountB; j++ )
				{
					BtU32 indexB = faceB.m_oldIndex[j];

					if( indexA == indexB )
					{
						v3AverageNormal += faceB.m_v3Normal;
					}
				}
			}

			// Set the new vertex 
			object.m_newVertex[faceA.m_newVertexStart + i].m_v3Normal = v3AverageNormal.GetNormalise();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// CalculateSmoothedNormals

void MCVanilla::CalculateSmoothedNormals( MCObject& object )
{
	ErrorLog::Printf( "CalculateSmoothedNormals" );

	// Get the number of faces
	BtU32 numVertex = (BtU32)object.m_oldVertex.size();

	for( BtU32 vertex=0; vertex<numVertex; vertex++ )
	{
		object.m_oldVertex[vertex].m_v3NewNormal = MtVector3( 0, 0, 0 );
	}

	// Get the number of faces
	BtU32 numFaces = (BtU32)object.m_faces.size();

	// Add all the normals to the face
	for( BtU32 iFaceA=0; iFaceA<numFaces; iFaceA++ )
	{
		// Cache each face
		MCFace& face = object.m_faces[iFaceA];

		BtU32 indexCount = (BtU32)face.m_oldIndex.size();

		// Flatten the normal
		for( BtU32 i=0; i<indexCount; i++ )
		{
			BtU32 index = face.m_oldIndex[i];
			object.m_oldVertex[index].m_v3NewNormal += face.m_v3Normal;
		}
	}

	// Normalise the normals
	for( BtU32 vertex=0; vertex<numVertex; vertex++ )
	{
		object.m_oldVertex[vertex].m_v3Normal = object.m_oldVertex[vertex].m_v3NewNormal.Normalise();
	}
}


////////////////////////////////////////////////////////////////////////////////
// Intermediate data
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// AddObject

void MCVanilla::AddObject( const MCObject& object )
{
	m_objects.push_back( object );
}

////////////////////////////////////////////////////////////////////////////////
// AddMaterial

void MCVanilla::AddMaterial( const MCMaterial& material )
{
	m_materials.push_back( material );
}

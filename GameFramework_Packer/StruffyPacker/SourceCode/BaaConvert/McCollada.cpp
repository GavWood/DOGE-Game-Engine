////////////////////////////////////////////////////////////////////////////////
// MCCollada.cpp

// Includes
#include <stdio.h>
#include "BtString.h"
#include "BtTypes.h"
#include "ErrorLog.h"

#include "FCollada.h"
#include "FCDAsset.h"
#include "FCDocument.h"
#include "FCDController.h"
#include "FCDEffect.h"
#include "FCDEffectProfileFX.h"
#include "FCDEffectStandard.h"
#include "FCDLibrary.h"
#include "FCDLight.h"
#include "FCDGeometry.h"
#include "FCDGeometryInstance.h"
#include "FCDGeometryMesh.h"
#include "FCDGeometryPolygons.h"
#include "FCDGeometryPolygonsInput.h"
#include "FCDGeometryPolygonsTools.h"
#include "FCDGeometrySource.h"
#include "FCDSceneNode.h"
#include "FCDImage.h"
#include "FCDTransform.h"
#include "FCDMaterialInstance.h"
#include "FCDMaterial.h"
#include "FCDMorphController.h"
#include "FCDPhysicsMaterial.h"
#include "FCDPhysicsModelInstance.h"
#include "FCDPhysicsRigidBody.h"
#include "FCDPhysicsRigidBodyInstance.h"
#include "FCDPhysicsRigidBodyParameters.h"
#include "FCDPhysicsScene.h"
#include "FCDPhysicsShape.h"
#include "FCDSkinController.h"
#include "FCDTexture.h"
#include "FUFileManager.h"
#include "FMMatrix44.h"
#include "McCollada.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

MCCollada::MCCollada()
{
}

////////////////////////////////////////////////////////////////////////////////
// Create

void MCCollada::Create()
{
	// Create a new top document
	m_pDocument = FCollada::NewTopDocument();

	// Make a new asset
	FCDAsset* pAsset = m_pDocument->GetAsset();

	// Make a contributor
	FCDAssetContributor* pContributor = pAsset->AddContributor();

	// Set the author
	pContributor->SetAuthor( "www.baawolf.com" );

	// Set the comments
	pContributor->SetComments( "EULA: Non commercial use only. Please contact sales at baawolf.com for commercial use." );

	// Add the visual scene
	m_pVisualScene = m_pDocument->AddVisualScene();

	// Extract the materials
	ExtractMaterials();
}

////////////////////////////////////////////////////////////////////////////////
// ExtractMaterials

void MCCollada::ExtractMaterials()
{
	// Cache the number of materials
	BtU32 numMaterials = m_pVanilla->GetNumMaterials();

	// Loop through the materials
	for( BtU32 iMaterial=0; iMaterial<numMaterials; iMaterial++ )
	{
		// Cache the material
		const MCMaterial& material = m_pVanilla->GetMaterial( iMaterial );

		// Make an effect
		FCDEffect* pEffect = m_pDocument->GetEffectLibrary()->AddEntity();

		// Add an effect profile
		FCDEffectStandard* pEffectProfile = (FCDEffectStandard*)pEffect->AddProfile( FUDaeProfileType::COMMON );

		// Set the lighting type to lambert
		pEffectProfile->SetLightingType( FCDEffectStandard::LAMBERT );

		if( BtStrLength( material.m_diffuseFilename ) > 0 )
		{
			// Add a texture
			FCDTexture* pTexture = pEffectProfile->AddTexture( FUDaeTextureChannel::DIFFUSE );

			// Make an effect
			FCDImage* pImage = m_pDocument->GetImageLibrary()->AddEntity();

			// Set the texture name
			pImage->SetFilename( material.m_diffuseFilename );

			// Set the image
			pTexture->SetImage( pImage );
		}

		// Set the diffuse colour
		pEffectProfile->SetDiffuseColor( FMVector4( material.m_colour.Red(),
													material.m_colour.Green(),
													material.m_colour.Blue(),
													material.m_colour.Alpha() ) );

		// Make some geometry
		FCDMaterial* pMaterial = m_pDocument->GetMaterialLibrary()->AddEntity();

		// Define the material id
		fstring materialId = material.m_name;

		// Set the material id
		pMaterial->SetDaeId( materialId );

		// Find the effect
		pMaterial->SetEffect( pEffect );

		// Add the material
		m_pMaterials.push_back( pMaterial );
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractObject

void MCCollada::ExtractObject()
{
	FCDSceneNode* pParents[8];

	// Cache the number of objects
	BtU32 numObjects = m_pVanilla->GetNumObjects();

	for( BtU32 iObject=0; iObject<numObjects; iObject++ )
	{
		// Cache each object
		const MCObject& object = m_pVanilla->GetObject( iObject );

		// Set the current node to NULL
		pSceneNode = BtNull;

		// Set the root
		if( object.m_depth == 0 )
		{	
			// Add a child node of the scene
			pSceneNode = m_pVisualScene->AddChildNode();
		}
		else
		{
			// Get the current parent
			FCDSceneNode* pCurrentParent = pParents[object.m_depth - 1];

			// Add a child node of the previous node
			pSceneNode = pCurrentParent->AddChildNode();
		}

		// Set the parent
		pParents[object.m_depth] = pSceneNode;

		// Set the id
		pSceneNode->SetDaeId( object.m_name );

		// Set the name
		pSceneNode->SetName( object.m_name );

		// Make some geometry
		FCDGeometry* pGeometry = m_pDocument->GetGeometryLibrary()->AddEntity();

		// Set the shape name
		BtChar shapeName[256];

		BtStrCopy( shapeName, 256, object.m_name );
		BtStrCat(  shapeName, 256, "Shape" );

		// Set the id
		pGeometry->SetDaeId( shapeName );

		// Set an appropriate name
		pGeometry->SetName( shapeName	);

		// Add the geometry to the instance
		pSceneNode->AddInstance( pGeometry );

		// Add the geometry mesh
		m_pGeometryMesh =  pGeometry->CreateMesh();

		// Extract the vertex
		ExtractVertex( object );

		// Extract the polygons for this object
		ExtractPolygons( object );
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractVertex

void MCCollada::ExtractVertex( const MCObject& object )
{
	BtU32 stride = 3;

	// Add the positions for the triangle
	BtU32 numVertex = object.m_newVertex.size();

	float *vtxPos = new float[ numVertex * stride ];

	for( BtU32 iVertex=0; iVertex<numVertex; iVertex++ )
	{
		vtxPos[iVertex * stride + 0] = (float) object.m_newVertex[iVertex].m_v3Position.x;
		vtxPos[iVertex * stride + 1] = (float) object.m_newVertex[iVertex].m_v3Position.y;
		vtxPos[iVertex * stride + 2] = (float) object.m_newVertex[iVertex].m_v3Position.z;
	}

	// Set the vertex positions
	FCDGeometrySource *pPositionSource = m_pGeometryMesh->AddVertexSource( FUDaeGeometryInput::POSITION );
	pPositionSource->SetData( FloatList( vtxPos, numVertex * stride ), 3 );
	pPositionSource->SetDaeId( m_pGeometryMesh->GetDaeId() + "-positions" );
	pPositionSource->SetName( FC("position") );

	// Add the Colours for the triangle
	float *vtxColour = new float[ numVertex * stride ];

	for( BtU32 iVertex=0; iVertex<numVertex; iVertex++ )
	{
		vtxColour[iVertex * stride + 0] = 1.0f;
		vtxColour[iVertex * stride + 1] = 1.0f;
		vtxColour[iVertex * stride + 2] = 1.0f;
	}

	// Set the vertex colours
	FCDGeometrySource *pColourSource = m_pGeometryMesh->AddVertexSource( FUDaeGeometryInput::COLOR );
	pColourSource->SetData( FloatList( vtxColour, numVertex * stride ), 3 );
	pColourSource->SetDaeId( m_pGeometryMesh->GetDaeId() + "-colours" );
	pColourSource->SetName( FC("colour") );

	// Add the normals for the triangle
	float *vtxNormal = new float[ numVertex * stride ];

	for( BtU32 iVertex=0; iVertex<numVertex; iVertex++ )
	{
		vtxNormal[iVertex * stride + 0] = (float) object.m_newVertex[iVertex].m_v3Normal.x; 
		vtxNormal[iVertex * stride + 1] = (float) object.m_newVertex[iVertex].m_v3Normal.y; 
		vtxNormal[iVertex * stride + 2] = (float) object.m_newVertex[iVertex].m_v3Normal.z;
	}

	// Set the vertex normals
	FCDGeometrySource *pNormalSource = m_pGeometryMesh->AddVertexSource( FUDaeGeometryInput::NORMAL );
	pNormalSource->SetData( FloatList( vtxNormal, numVertex * stride ), stride );
	pNormalSource->SetDaeId( m_pGeometryMesh->GetDaeId() + "-normals" );
	pNormalSource->SetName( FC("normal") );

	stride = 2;

	// Add the normals for the triangle
	float *vtxUV = new float[ numVertex * stride ];

	for( BtU32 iVertex=0; iVertex<numVertex; iVertex++ )
	{
		vtxUV[iVertex * stride + 0] = (float) object.m_newVertex[iVertex].m_v2UV.x; 
		vtxUV[iVertex * stride + 1] = (float) object.m_newVertex[iVertex].m_v2UV.y; 
	}

	// Set the vertex UVs
	FCDGeometrySource *pUVSource = m_pGeometryMesh->AddVertexSource( FUDaeGeometryInput::TEXCOORD );
	pUVSource->SetData( FloatList( vtxUV, numVertex * stride ), stride );
	pUVSource->SetDaeId( m_pGeometryMesh->GetDaeId() + "-uvs" );
	pUVSource->SetName( FC("uv") );
}

////////////////////////////////////////////////////////////////////////////////
// ExtractPolygons

void MCCollada::ExtractPolygons( const MCObject& object )
{
	FCDGeometryPolygons* pPolygons;

	// Cache the number of materials
	BtU32 numMaterials = m_pVanilla->GetNumMaterials();

	// Loop through the materials

	for( BtU32 iMaterial=0; iMaterial<numMaterials; iMaterial++ )
	{
		BtBool found = BtFalse;

		// Cache the number of face
		BtU32 numFace = object.m_faces.size();

		for( BtU32 iFace=0; iFace<numFace; iFace++ )
		{
			// Cache each face
			const MCFace& face = object.m_faces[iFace];

			if( face.m_materialID == iMaterial )
			{
				found = BtTrue;
			}
		}

		if( found == BtTrue )
		{
			// Add the geometry polygons
			pPolygons = m_pGeometryMesh->AddPolygons();

			// Set the primitive type to triangle
			pPolygons->SetPrimitiveType( FCDGeometryPolygons::POLYGONS );

			// Create a geometry instance for this polygon
			FCDGeometryInstance* pGeometryInstance =(FCDGeometryInstance*)FCDEntityInstanceFactory::CreateInstance( m_pDocument, pSceneNode, FCDEntity::GEOMETRY );

			// Add the material instance
			pGeometryInstance->AddMaterialInstance( m_pMaterials[iMaterial], pPolygons );

			// Set the material
			pPolygons->SetMaterialSemantic( m_pMaterials[iMaterial]->GetDaeId() );

			// Cache the position source
			FCDGeometrySource *pPositionSource = m_pGeometryMesh->GetVertexSource( FUDaeGeometryInput::POSITION );

			// Cache the polygon input
			FCDGeometryPolygonsInput *pPolygonInput = pPolygons->FindInput( pPositionSource );

			// Cache the number of face
			BtU32 numFace = object.m_faces.size();

			// Make the faces
			for( BtU32 iFace=0; iFace<numFace; iFace++ )
			{
				// Cache each face
				const MCFace& face = object.m_faces[iFace];

				if( face.m_materialID == iMaterial )
				{
					// Cache the number of index per face
					BtU32 numIndex = face.m_newIndex.size();

					// Add the face
					pPolygons->AddFace( numIndex ); 
				}
			}

			// ErrorLog::Printf( "Added %d faces\r\n", numFace );

			BtU32 indexOffset = 0;

			uint32 *positionIndicies = pPolygonInput->GetIndices();

			if( positionIndicies != BtNull )
			{
				for( BtU32 iFace=0; iFace<numFace; iFace++ )
				{
					// Cache each face
					const MCFace& face = object.m_faces[iFace];

					if( face.m_materialID == iMaterial )
					{
						// Cache the number of index per face
						BtU32 numIndex = face.m_newIndex.size();

						if( m_pVanilla->GetReversedWindingOrder() == BtFalse )
						{
							for( BtU32 iIndex=0; iIndex<numIndex; iIndex++ )
							{
								positionIndicies[indexOffset] = face.m_newIndex[iIndex];

								++indexOffset;
							}
						}
						else
						{
							for( BtS32 iIndex=numIndex - 1; iIndex >= 0; iIndex-- )
							{
								positionIndicies[indexOffset] = face.m_newIndex[iIndex];

								++indexOffset;
							}
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Create

void MCCollada::Create( const MCVanilla* pVanilla )
{
	m_pVanilla = pVanilla;

	Create();
	ExtractObject();
}

////////////////////////////////////////////////////////////////////////////////
// Export

void MCCollada::Export( BtChar* fileName )
{
	// Save the document
	FCollada::SaveDocument( m_pDocument, fileName ); 
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void MCCollada::Destroy()
{
	// Release the document
	m_pDocument->Release();
}

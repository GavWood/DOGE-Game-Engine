///////////////////////////////////////////////////////////////////////////////
// ExScene.cpp

// Includes
#include "BtTypes.h"
#include "BtMemory.h"
#include "BtString.h"
#include "BaArchive.h"
#include "SgAnimator.h"
#include "RsColour.h"
#include "RsVertex.h"
#include "MtVector2.h"
#include "MtVector3.h"
#include "ExScene.h"
#include "ExMatrix.h"
#include "PaArchive.h"
#include "PaAsset.h"
#include "PaFileDetails.h"
#include "PaMemoryResource.h"
#include "ErrorLog.h"
#include "ExVertex.h"
#include "ExStrip.h"
#include "ExRigidBody.h"
#include "ExSgMesh.h"
#include "ExSgSkin.h"
#include "ExSgBone.h"
#include "ExSgCamera.h"
#include "ExSgLight.h"
#include "ExSgCollision.h"
#include "ExSgBlendShape.h"
#include "ExStandardShader.h"
#include "UtTokeniser.h"
#include "MCVanilla.h"
#include "MCMetasequoia.h"
#include "MCCollada.h"
#include "PaTopState.h"
#include "RsIndexBuffer.h"
#include "FsFile.h"

//#ifdef WIN32
#include "StdAfx.h"
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
//#endif

////////////////////////////////////////////////////////////////////////////////
// Globals

BtFloat UnitConversion = 1.0f / 100.0f;
BtBool ExScene::m_isMetasequoia;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExScene::ExScene()
{
}

////////////////////////////////////////////////////////////////////////////////
// Create

// virtual
void ExScene::Create()
{
	m_isMetasequoia = BtFalse;
	m_pDocument = BtNull;
	m_maxAnimationDuration = 0;
	//m_animationFrameRate = 25.0f;
	m_animationFrameRate = 24.0f;
	m_isDynamicVB = BtFalse;
	// m_animationFrameRate = 50.0f;

	BtU32 MaxVertexDeclaration = VT_Position | VT_Normal | VT_Colour | VT_Tangent | VT_Tangent | VT_UV0 | VT_UV1 | VT_Skin | VT_BlendShape;

	if( MaxVertexDeclaration >= MaxVertexDeclarations )
	{
		ErrorLog::Fatal_Printf( "Number of vertex declarations exceeded.\r\n" );
	}
}

////////////////////////////////////////////////////////////////////////////////
// FindSceneNodeById

ExSgNode* ExScene::FindSceneNodeById( const BtChar* sceneNodeId )
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		if( BtStrCompare( sceneNodeId, pSceneNode->m_id ) == BtTrue )
		{
			return pSceneNode;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// FindSceneNode

ExSgNode* ExScene::FindSceneNode( const BtChar* sceneNodeName )
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		if( BtStrCompare( sceneNodeName, pSceneNode->m_fileData.m_name ) == BtTrue )
		{
			return pSceneNode;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// SetExportHierarchy

void ExScene::SetExportHierarchy()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		// Set the export hierarchy
		pSceneNode->SetExportHierarchy();
	}
}
////////////////////////////////////////////////////////////////////////////////
// Update

void ExScene::Update()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		// Update each scene node
		pSceneNode->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractHierarchy

void ExScene::ExtractHierarchy( FCDSceneNode* pSceneNode, ExSgNode* pCurrentNode )
{
	// Cache the number of children
	BtU32 nChildren = (BtU32) pSceneNode->GetChildrenCount();

	// Recurse through the children
	for( BtU32 nChild = 0; nChild<nChildren; nChild++ )
	{
		// Create a new node
		ExSgNode* pChildNode = new ExSgNode( this );

		// Get the child Collada node
		FCDSceneNode* pChildColladaNode = pSceneNode->GetChild(nChild);

			
		if( pChildColladaNode->IsJoint() )
		{
			int a = 0;
			a++;
		}

		// Cache the name
		const BtChar* pChildName = &pChildColladaNode->GetName()[0];

		// Remove the default Maya cameras if they can be found
		if( ( BtStrCompare( pChildName, "persp" ) == BtTrue ) ||
			( BtStrCompare( pChildName, "top" ) == BtTrue ) ||
			( BtStrCompare( pChildName, "side" ) == BtTrue ) || 
			( BtStrCompare( pChildName, "front" ) == BtTrue ) ||
			( BtStrCompare(pChildName, "left") == BtTrue) ||
			( BtStrCompare(pChildName, "bottom") == BtTrue) )
		{
			continue;
		}

			// Cache the file data
		BaSgNodeFileData& fileData = pChildNode->m_fileData;

		// Set the Collada node
		pChildNode->m_pColladaNode = pChildColladaNode;

		// Set the name
		BtStrCopy( pChildNode->m_id, MaxSceneNodeName, &pChildNode->m_pColladaNode->GetSubId()[0] );

		// Set the name
		BtStrCopy( fileData.m_name, MaxSceneNodeName, &pChildNode->m_pColladaNode->GetName()[0] );

		// Count the number of nodes
		BtU32 count = m_pSgNodes.size();

		// Display the node name
		ErrorLog::Printf( "Node %d name \"%s\"", count, fileData.m_name );

		BtU32 instanceCount = pChildNode->m_pColladaNode->GetInstanceCount();
		(void)(instanceCount);

		// Add the node
		AddSceneNode( pChildNode );

		// Set the local matrix
        FMMatrix44 m4OriginalTransform = pChildColladaNode->ToMatrix();
		MtMatrix4 m4Transform;
		BtMemory::Copy( &m4Transform, (void*)&m4OriginalTransform, sizeof(MtMatrix4) );
		pChildNode->Local(m4Transform);

		// Add each child
		if( pCurrentNode != BtNull )
		{
			pCurrentNode->m_children.push_back( pChildNode );
		}

		// Extract the hierarchy
		ExtractHierarchy( pChildColladaNode, pChildNode );
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetAnimationLength

void ExScene::GetAnimationLength()
{
	m_maxAnimationDuration = 0;

	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		m_pCurrentSceneNode = m_pSgNodes[nSceneNode];

		// Extract any animation
		m_maxAnimationDuration = MtMax( m_maxAnimationDuration, m_pCurrentSceneNode->GetAnimationLength() );
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractSceneNodes

void ExScene::ExtractSceneNodes()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		m_pCurrentSceneNode = m_pSgNodes[nSceneNode];

		BtChar *name = m_pCurrentSceneNode->pName();

		// Extract any animation
		m_pCurrentSceneNode->ExtractAnimation( m_maxAnimationDuration, m_animationFrameRate );

		// Cache the number of instances
		BtU32 nInstances = (BtU32) m_pCurrentSceneNode->m_pColladaNode->GetInstanceCount();

		ErrorLog::Printf("Instances for %s=%d", name, nInstances);

		// Loop through the instances
		for( BtU32 nInstanceIndex=0; nInstanceIndex<nInstances; nInstanceIndex++ )
		{
			// Cache the entity instance
			FCDEntityInstance* pEntityInstance = m_pCurrentSceneNode->m_pColladaNode->GetInstance(nInstanceIndex);

			switch( pEntityInstance->GetType() )
			{
				case FCDEntityInstance::SIMPLE:
					{
						// Get the controller
						FCDController* pController = (FCDController*) pEntityInstance->GetEntity();

						if( pController->GetType() == FCDController::CAMERA )
						{
							// Create a camera
							m_pCurrentSceneNode->m_pCamera = new ExSgCamera( m_pCurrentSceneNode, this );

							// Set the node type
							m_pCurrentSceneNode->SetNodeType( ExSgType_Camera );
						}
						else if( pController->GetType() == FCDController::LIGHT )
						{
							// Extract the light
							ExtractLight( pEntityInstance );
						}
						else
						{
							int a = 0;
							a++;
						}
					}
					break;

				case FCDEntityInstance::GEOMETRY:

					// Extract the geometry
					ExtractGeometry( pEntityInstance );
					break;

				case FCDEntityInstance::CONTROLLER:

					// Get the controller
					FCDController* pController = (FCDController*) pEntityInstance->GetEntity();

					if( pController->IsSkin() == BtTrue )
					{
						// Cache the skin controller
						FCDSkinController* pSkinController = (FCDSkinController*) pController->GetSkinController();

						// Create the skin
						m_pCurrentSceneNode->m_pSkin = new ExSgSkin( m_pCurrentSceneNode, this );

						// Extract the bones
						ExtractBones( pSkinController );

						// Extract the skin
						ExtractSkin( pEntityInstance );

						// Get the geometry
						FCDGeometry* pGeometry = pController->GetBaseGeometry();

						// Extract the geometry
						ExtractGeometry( (FCDGeometryInstance*) pEntityInstance, pGeometry );
					}
					else if( pController->IsMorph() == BtTrue )
					{
						// Extract the blend shape
						ExtractBlendShape( pEntityInstance, pController );
					}
					break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// TranslateVertices

void ExScene::TranslateVertices( ExSgNode* pNode, MtMatrix4 m4Transform )
{
	// Cache the mesh
	ExSgMesh* pMesh = pNode->m_pMesh;

	if( pMesh != BtNull )
	{
		// Cache the number of material blocks
		BtU32 numMaterialBlocks = pMesh->m_materialBlocks.size();

		for( BtU32 iMaterial=0; iMaterial<numMaterialBlocks; iMaterial++ )
		{
			// Cache each material block
			ExMaterialBlock* pMaterialBlock = pMesh->m_materialBlocks[iMaterial];

			// Cache the number of vertex
			BtU32 numVertex = pMaterialBlock->m_pVertex.size();

			// Loop through the vertex
			for( BtU32 iVertex=0; iVertex<numVertex; iVertex++ )
			{
				// Cache the vertex positions
				MtVector3 v3Position = pMaterialBlock->m_pVertex[iVertex]->Position();

				// Translate the hinged vertices
				v3Position = v3Position * m4Transform;

				// Reset their positions
				pMaterialBlock->m_pVertex[iVertex]->Position( v3Position );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractHinges

void ExScene::ExtractHinges()
{
	// Cache the number of scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	// Loop through the scene nodes
	for( BtU32 iSceneNode=0; iSceneNode<nSceneNodes; iSceneNode++ )
	{
		// Cache each node
		ExSgNode* pNode = m_pSgNodes[iSceneNode];

		if( BtStrStr( pNode->pName(), "Hinge") != BtNull )
		{
			// Cache the mesh
			ExSgMesh* pMesh = pNode->m_pMesh;

			// Cache the number of material blocks
			BtU32 numMaterialBlocks = pMesh->m_materialBlocks.size();

			for( BtU32 iMaterial=0; iMaterial<numMaterialBlocks; iMaterial++ )
			{
				// Cache each material block
				ExMaterialBlock* pMaterialBlock = pMesh->m_materialBlocks[iMaterial];

				MtTriangle triangle1( pMaterialBlock->m_pVertex[0]->Position(),
									  pMaterialBlock->m_pVertex[1]->Position(),
									  pMaterialBlock->m_pVertex[2]->Position() );

				// Find the normal
				triangle1.FindNormal();

				// Find the center
				triangle1.FindPolygonCenter();

				MtTriangle triangle2;

				if( pMaterialBlock->m_indices.size() >= 6 )
				{
					triangle2 = MtTriangle( pMaterialBlock->m_pVertex[3]->Position(),
											pMaterialBlock->m_pVertex[4]->Position(),
											pMaterialBlock->m_pVertex[5]->Position() );
				}
				else
				{
					triangle1 = triangle2;
				}

				// Find the normal
				triangle2.FindNormal();

				// Find the center
				triangle2.FindPolygonCenter();

				MtVector3 v3Up = triangle2.center - triangle1.center;

				v3Up.Normalise();
									  
				// Set the translation
				MtMatrix4 m4Transform;

				// Look down the normal
				m4Transform.LookAt( triangle1.center, triangle1.center - triangle2.normal, v3Up );

				// Set the local transform
				pNode->Local( m4Transform );

				// Translate the vertices as they will take the transform from the parent node
				TranslateVertices( pNode->m_pFirstChild, m4Transform.GetInverse() );

				int a=0;
				a++;
			}
			int a=0;
			a++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractPhysicsNodes

void ExScene::ExtractPhysicsNodes()
{
	// Cache the scene node
	FCDPhysicsScene* pSceneNode = m_pDocument->GetPhysicsSceneInstance(0);

	if( pSceneNode == BtNull )
	{
		return;
	}

	// Cache the number of instances
	BtU32 nInstances = (BtU32)pSceneNode->GetPhysicsModelInstancesCount();

	// Iterate through the instances
	for( BtU32 iInstance=0; iInstance<nInstances; iInstance++ )
	{
		FCDPhysicsModelInstance* pPhysicsModel = pSceneNode->GetPhysicsModelInstance( iInstance );

		if( pPhysicsModel->GetType() == FCDEntityInstance::PHYSICS_MODEL )
		{
			BtU32 nInstances = (BtU32)pPhysicsModel->GetInstanceCount();

			if( nInstances > 0 )
			{
				// Loop through the instances
				for( BtU32 nInstanceIndex=0; nInstanceIndex<nInstances; nInstanceIndex++ )
				{
					// Cache the entity instance
					const FCDEntityInstance* pEntityInstance = pPhysicsModel->GetInstance( nInstanceIndex );

					// Cache each entities type
					FCDPhysicsModelInstance::Type Type = pEntityInstance->GetType();

					if( Type == FCDEntityInstance::PHYSICS_RIGID_BODY )
					{
						// Cache the rigid body instance
						FCDPhysicsRigidBodyInstance* pRigidBodyInstance = (FCDPhysicsRigidBodyInstance*) pEntityInstance;

						// Cache the scene node which this physics body targets
						FCDSceneNode* pSceneNode = pRigidBodyInstance->GetTargetNode();

						// Find this node
						ExSgNode* pNode = FindSceneNode( pSceneNode );

						BtAssert( pNode != BtNull );

						// Flag the type as a physics node
						pNode->SetNodeType( ExSgType_RigidBody );

						// Set the entity instance
						pNode->m_pRigidBodyInstance = pRigidBodyInstance;
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExportPhysicsNodes

void ExScene::ExportPhysicsNodes()
{
	BtU32 numSceneNodes = GetNumSceneNodes();

	for( BtU32 iNode=0; iNode<numSceneNodes; ++iNode )
	{
		// Read in the physics configuration file

		// Cache each node
		ExSgNode* pNode = m_pSgNodes[iNode];

		BtBool isConvexHull    = BtFalse;
		BtBool isCollisionMesh = BtFalse;
		BtFloat mass = 0;
		BtBool isMass = BtFalse;

		if( ( pNode->GetNodeType( ExSgType_RigidBody ) == BtTrue ) ||
			( pNode->GetNodeType( ExSgType_Mesh ) == BtTrue ) )
		{
			BtChar dynamicsFilename[256];

			// Set the material filename
			sprintf( dynamicsFilename, "%s\\%s.config", pPath(), pNode->GetTitle() );

			UtTokeniser tokeniser;

			if( tokeniser.Read( dynamicsFilename ) == BtTrue )
			{
				//ErrorLog::Printf( "Read dynamics config file for %s\r\n", dynamicsFilename );

				tokeniser.GetBool( "convexhull", isConvexHull );
				tokeniser.GetBool( "collisionMesh", isCollisionMesh );
				tokeniser.GetBool( "dynamicvb", m_isDynamicVB );
				isMass = tokeniser.GetFloat( "mass", mass );
				
				pNode->SetNodeType( ExSgType_RigidBody );
			}
		}

		if( pNode->GetNodeType( ExSgType_RigidBody ) == BtTrue )
		{
			// Set the current scene node
			m_pCurrentSceneNode = pNode;

			// Make a rigid body
			ExRigidBody* pRigidBody = new ExRigidBody( m_pCurrentSceneNode, this );

			FCDPhysicsRigidBodyInstance* pRigidBodyInstance = pNode->m_pRigidBodyInstance;

			if( pRigidBodyInstance != BtNull )
			{
				FCDPhysicsRigidBody* pRigidBodyEntity = pRigidBodyInstance->GetRigidBody();
				const FCDPhysicsRigidBodyParameters* pParams = pRigidBodyEntity->GetParameters();

				// Cache the scene node which this physics body targets
				FCDSceneNode* pSceneNode = pRigidBodyInstance->GetTargetNode();

				// Find this node
				m_pCurrentSceneNode = FindSceneNode( pSceneNode );

				// Validate back the other way. This ensures physics node is attached to hierarchy node
				BtAssert( m_pCurrentSceneNode == pNode );

				// Validate the node
				BtAssert( m_pCurrentSceneNode != BtNull );

				// Set the rigid body against the scene node
				m_pCurrentSceneNode->m_pRigidBody = pRigidBody;

				// Set the scene node type
				m_pCurrentSceneNode->SetNodeType( ExSgType_RigidBody );

				// Cache the physics material
				const FCDPhysicsMaterial* pMaterial = pParams->GetPhysicsMaterial();

				// Set the friction
				pRigidBody->Friction( pMaterial->GetDynamicFriction() );

				// Set the restitution
				pRigidBody->Restitution( pMaterial->GetRestitution() );

				if( isMass == BtTrue )
				{
					pRigidBody->Mass( mass );
				}
				else
				{
					if( pParams->GetDynamic() > 0.0f )
					{
						pRigidBody->IsDynamic( BtTrue );

						// Set the parameters
						pRigidBody->Mass( pParams->GetMass() );
					}
					else
					{
						pRigidBody->IsDynamic( BtFalse );

						// Set the parameters
						pRigidBody->Mass( 0 );
					}
				}

				// Get the primitive shapes count
				BtU32 numShapes = (BtU32)pParams->GetPhysicsShapeCount();

				if( numShapes )
				{
					// Make a collision body
					ExSgCollision* pCollision = new ExSgCollision( m_pCurrentSceneNode, this );

					// Set the collision body against the scene node
					m_pCurrentSceneNode->m_pCollision = pCollision;

					// Set the scene node type
					m_pCurrentSceneNode->SetNodeType( ExSgType_Collision );

					// Begin the shapes
					pCollision->Begin( m_collisionShapes.NumShapes() );

					numShapes = 1;

					// Loop through the shapes
					for( BtU32 iShape=0; iShape<numShapes; iShape++ )
					{
						// Get each shape in turn
						const FCDPhysicsShape* pPhysicsShape = pParams->GetPhysicsShape( iShape );

						// Validate the physics shape
						BtAssert( pPhysicsShape != NULL );

						// Cache the analytical geometry
						const FCDPhysicsAnalyticalGeometry* pGeometry = pPhysicsShape->GetAnalyticalGeometry();

						// Get the transforms
						FMMatrix44 Transform( FMMatrix44::Identity );

						// Cache the transforms
						const FCDTransformContainer& Transforms = pPhysicsShape->GetTransforms();

						// Cache the number of transforms
						BtU32 TransformCount = (BtU32) Transforms.size();

						// Iterate through the transforms
						for( BtU32 iTransform = 0; iTransform<TransformCount; iTransform++ )
						{
							// Cache each transforms
							const FCDTransform* pTransform = Transforms[iTransform];

							// Multiply each transform
							Transform = Transform * pTransform->ToMatrix();
						}

						// Copy the transform
						MtMatrix4 m4Transform;
						BtMemory::Copy( &m4Transform, &Transform, sizeof(MtMatrix4) );
						
						if( pGeometry != BtNull )
						{
							FCDPhysicsAnalyticalGeometry::GeomType geomType = pGeometry->GetGeomType();

							// Hack
							if( geomType == FCDPhysicsAnalyticalGeometry::BOX )
							{
								if( BtStrStr( m_pCurrentSceneNode->pName(), "Cylinder" ) )
								{
									geomType = FCDPhysicsAnalyticalGeometry::CYLINDER;
								}
							}

							switch( geomType )
							{
							case FCDPhysicsAnalyticalGeometry::BOX:
								{
									// Cache each box
									const FCDPASBox* pBox = (FCDPASBox*) pPhysicsShape->GetAnalyticalGeometry();

									// Cache the half extents
									FMVector3 extents = pBox->halfExtents * UnitConversion;

									// Add a box
									m_collisionShapes.AddBox( m4Transform, extents.x, extents.y, extents.z );
								}
								break;
							case FCDPhysicsAnalyticalGeometry::SPHERE:
								{
									FCDPASSphere* pSphere = (FCDPASSphere*) pPhysicsShape->GetAnalyticalGeometry();
									BtFloat radius = pSphere->radius * UnitConversion;

									// Add a sphere
									m_collisionShapes.AddSphere( m4Transform, radius );
								}
								break;
							case FCDPhysicsAnalyticalGeometry::PLANE:
								{
									FCDPASPlane* pPlane = (FCDPASPlane*) pPhysicsShape->GetAnalyticalGeometry();
									FMVector3 normal = pPlane->normal;
			//						Shape.Normal_ = BcVec3d( normal.x, normal.y, normal.z );
			//						Shape.Distance_ = pPlane->d;
								}
								break;
							case FCDPhysicsAnalyticalGeometry::CYLINDER:
								{
									// Cache each box
									const FCDPASBox* pBox = (FCDPASBox*) pPhysicsShape->GetAnalyticalGeometry();

									BtFloat radius = pBox->halfExtents.x * UnitConversion;
									BtFloat length = pBox->halfExtents.y * UnitConversion;

									// Add a cylinder
									m_collisionShapes.AddCylinder( m4Transform, length, radius );

									//FCDPASCylinder* pCyl = (FCDPASCylinder*) pPhysicsShape->GetAnalyticalGeometry();
								}
								break;
							case FCDPhysicsAnalyticalGeometry::CAPSULE:
								{
									// FCDPASCapsule* pCap = (FCDPASCapsule*) pPhysicsShape->GetAnalyticalGeometry();
								}
								break;
							case FCDPhysicsAnalyticalGeometry::TAPERED_CYLINDER:
								{
									// FCDPASTaperedCylinder* pCyl = (FCDPASTaperedCylinder*) pPhysicsShape->GetAnalyticalGeometry();
								}
								break;
							case FCDPhysicsAnalyticalGeometry::TAPERED_CAPSULE:
								{
									// FCDPASTaperedCapsule* pCap = (FCDPASTaperedCapsule*) pPhysicsShape->GetAnalyticalGeometry();
								}
								break;
							}
							int a=0;
							a++;
						}
						else
						{
							BtU32 startVertex = m_collisionMesh.NumVertex();
							BtU32 startIndex  = m_collisionMesh.NumIndex();

							// Extract the mesh
							m_collisionMesh.ExtractMesh( m_pCurrentSceneNode );

							BtU32 endVertex = m_collisionMesh.NumVertex();
							BtU32 endIndex  = m_collisionMesh.NumIndex();

							// Add the mesh
							if( isConvexHull == BtFalse )
							{
								m_collisionShapes.AddMesh( pNode->m_fileData.m_name,
														   m4Transform,
														   startIndex, 
														   endIndex,
														   startVertex,
														   endVertex );
							}
							else
							{
								// Add the convex hull
								m_collisionShapes.AddConvexHull( pNode->m_fileData.m_name,
														         m4Transform,
																 startIndex, 
																 endIndex,
																 startVertex,
																 endVertex );
							}
						}
					}

					// End the shapes
					pCollision->End( m_collisionShapes.NumShapes() );
				}
			}
			else
			{
				pRigidBody->Friction( 0 );
				pRigidBody->Restitution( 0 );
				pRigidBody->Mass( 0 );
		
				m_pCurrentSceneNode = pNode;

				// Set the rigid body against the scene node
				m_pCurrentSceneNode->m_pRigidBody = pRigidBody;

				// Make a collision body
				ExSgCollision* pCollision = new ExSgCollision( m_pCurrentSceneNode, this );

				// Set the collision body against the scene node
				m_pCurrentSceneNode->m_pCollision = pCollision;

				// Set the scene node type
				m_pCurrentSceneNode->SetNodeType( ExSgType_Collision );

				BtU32 count = m_collisionShapes.NumShapes();

				// Begin the shapes
				pCollision->Begin( count );

				// Copy the transform
				MtMatrix4 m4Transform;
				BtMemory::Copy( &m4Transform, &m_pCurrentSceneNode->World(), sizeof(MtMatrix4) );

				BtU32 startVertex = m_collisionMesh.NumVertex();
				BtU32 startIndex  = m_collisionMesh.NumIndex();

				// Extract the mesh
				m_collisionMesh.ExtractMesh( m_pCurrentSceneNode );

				BtU32 endVertex = m_collisionMesh.NumVertex();
				BtU32 endIndex  = m_collisionMesh.NumIndex();

				// Add the mesh
				if( isConvexHull == BtFalse )
				{
					m_collisionShapes.AddMesh( 
						pNode->m_fileData.m_name,
						m4Transform,
						startIndex, 
						endIndex,
						startVertex,
						endVertex );
				}
				else
				{
					// Add the convex hull
					m_collisionShapes.AddConvexHull(
						pNode->m_fileData.m_name,
						m4Transform,
						startIndex, 
						endIndex,
						startVertex,
						endVertex );
				}

				// End the shapes
				pCollision->End( m_collisionShapes.NumShapes() );

				BtChar text[64];
				sprintf( text, "count=%d start %d end %d\n", count, startVertex, endVertex );
				ErrorLog::Printf( text );

				int a=0;
				a++;
			}
		}
	}
}

// HACK
ExSgNode *ExScene::FindRoot( ExSgNode *pNode )
{
	if( pNode->m_pParent )
	{
		if( pNode->m_pParent->GetNodeType( ExSgType_Bone ) == BtTrue )
		{
			return FindRoot( pNode->m_pParent );
		}
	}
	return pNode;
}

////////////////////////////////////////////////////////////////////////////////
// ExtractBones

void ExScene::ExtractBones( const FCDSkinController* pSkinController )
{
	// Cache the skin
	ExSgSkin* pSkin = (ExSgSkin*)m_pCurrentSceneNode->m_pSkin;	
	
	// Get the list of bind poses
	BtU32 nJointCount = (BtU32)pSkinController->GetJointCount();

	pSkin->m_numJoints = nJointCount;

	ErrorLog::Printf("Found %d joints.\n", nJointCount);

	// Loop through the joints
	for (BtU32 iJoint = 0; iJoint < nJointCount; iJoint++)
	{
		// Cache each joint
		const FCDSkinControllerJoint* pJoint = pSkinController->GetJoint(iJoint);

		// Get the scene node
		fm::string id = pJoint->GetId();

		// Find the scene node
		ExSgNode* pSceneNode = FindSceneNodeById(id);

		// Validate the scene node
		BtAssert(pSceneNode != BtNull);

		BtPrint("Old joint %d = node %s", iJoint, pSceneNode->m_fileData.m_name);

		// Set the skeleton lookup table 
		pSkin->m_skeleton[iJoint] = pSceneNode;

		// Get the bind pose
		const FMMatrix44& fm4BindPose = pJoint->GetBindPoseInverse();

		MtMatrix4 m4InverseBindPose;

		// Copy the inverse bind pose
		BtMemory::Copy(&m4InverseBindPose, fm4BindPose, sizeof(FMMatrix44));

		// Create a bone
		pSceneNode->m_pBone = new ExSgBone(pSceneNode,		// Pointer to the scene node
			this,
			iJoint
			);		// Pointer to the scene

		// Debug the bone
		//ErrorLog::Printf( "Setting node \"%s\" as bone\r\n", pSceneNode->m_fileData.m_name );

		// Set the new node type
		pSceneNode->SetNodeType(ExSgType_Bone);

		// Set the inverse bind pose
		pSceneNode->m_pBone->InverseBindPose(m4InverseBindPose);

		// Set the bone index
 		pSceneNode->m_pBone->BoneIndex(iJoint);
	}
}

////////////////////////////////////////////////////////////////////////////////
// FindSceneNode

ExSgNode* ExScene::FindSceneNode( const FCDSceneNode* pColladaNode )
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		if( pSceneNode->m_pColladaNode == pColladaNode )
		{
			// Cache the scene node name
			fm::string sceneNodeName = pColladaNode->GetName();
			BtChar Name[256];
			BtStrCopy( Name, sizeof(Name), (BtChar*)&sceneNodeName[0] );

			// Validate the found node
			BtAssert( BtStrCompare( Name, pSceneNode->pName() ) == BtTrue );

			// Return the node
			return pSceneNode;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// ExtractBlendShape

void ExScene::ExtractBlendShape( FCDEntityInstance* pEntityInstance, FCDController* pController )
{
	// Create the morph target
	m_pCurrentSceneNode->m_pBlendShape = new ExSgBlendShape( m_pCurrentSceneNode, this );

	m_pCurrentSceneNode->SetNodeType( ExSgType_TargetBlendShape );
	m_pCurrentSceneNode->SetNodeType( ExSgType_Mesh );

	// Cache the geometry instance
	FCDGeometryInstance* pGeometryInstance = (FCDGeometryInstance*) pEntityInstance;

	// Get the base geometry
	FCDGeometry* pGeometry = pController->GetBaseGeometry();

	// Extract the geometry
	ExtractGeometry( pGeometryInstance, pGeometry );

	// Cache the morph target controller
	const FCDMorphController* pMorphTarget = (FCDMorphController*) pController->GetMorphController();

	// Cache the target count
	BtU32 nItems = (BtU32) pMorphTarget->GetTargetCount();

	// Link the blend shapes
	for( BtU32 iTarget=0; iTarget<nItems; iTarget++ )
	{					
		const FCDMorphTarget* pTarget = pMorphTarget->GetTarget( iTarget );

		// Cache the morph target geometry
		const FCDGeometry* pMorphTargetGeometry = pTarget->GetGeometry();

		// Get the scene node
		//fm::string id = pMorphTargetGeometry->GetDaeId();

		// Find the scene node		
		//ExSgNode* pNode = FindSceneNodeById( id );

		// Cache the target node name
		const BtChar* pName = &pMorphTargetGeometry->GetName()[0];

		// Find the scene node		
		ExSgNode* pNode = FindSceneNodeById( pName );

		if( pNode == BtNull )
		{
			ErrorLog::Fatal_Printf( "ExtractBlendShape:: Cannot find node %s\r\n", pName );
		}

		// Flag each of these as blend shapes
		pNode->SetNodeType( ExSgType_SourceBlendShape );

		// Add the blend shapes
		m_pCurrentSceneNode->m_pBlendShape->AddBlendShape( pNode );
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractSkin

void ExScene::ExtractSkin( const FCDEntityInstance* pEntityInstance )
{
	// Set the type
	m_pCurrentSceneNode->SetNodeType( ExSgType_Skin );

	// Extract the joint weight pairs
	m_pCurrentSceneNode->ExtractJointWeightPairs( pEntityInstance );
}

////////////////////////////////////////////////////////////////////////////////
// ExtractGeometry

void ExScene::ExtractGeometry( FCDEntityInstance* pEntityInstance )
{
	// Cache the geometry instance
	FCDGeometryInstance* pGeometryInstance = (FCDGeometryInstance*) pEntityInstance;

	// Get the geometry entity
	FCDGeometry* pGeometry = (FCDGeometry*) pGeometryInstance->GetEntity();

	// Set the type
	m_pCurrentSceneNode->SetNodeType( ExSgType_Mesh );

	// Extract the geometry
	ExtractGeometry( pGeometryInstance, pGeometry );
}

////////////////////////////////////////////////////////////////////////////////
// Extract the light

void ExScene::ExtractLight( FCDEntityInstance* pEntity )
{
	// Set the node type
	m_pCurrentSceneNode->SetNodeType( ExSgType_Light );

	// Create a new light
	ExSgLight* pLight = new ExSgLight( m_pCurrentSceneNode, this );

	// Create a mesh
	m_pCurrentSceneNode->m_pLight = pLight;

	// Cache the Collada light
	FCDLight* pColladaLight = (FCDLight*) pEntity->GetEntity();

	// Get the light type
	FCDLight::LightType lightType = pColladaLight->GetLightType();

	switch( lightType )
	{
		case FCDLight::DIRECTIONAL:

			pLight->m_type = RsLT_Direction;
			break;

		case FCDLight::POINT:

			pLight->m_type = RsLT_Point;
			pLight->m_fallOffAngle = 0;
			pLight->m_outerAngle = 0;
			break;

		case FCDLight::SPOT:

			pLight->m_type = RsLT_Cone;

			// Get the fall off angle
			pLight->m_fallOffAngle = pColladaLight->GetFallOffAngle();

			// Get the outer fall off angle
			pLight->m_outerAngle = pColladaLight->GetOuterAngle();
			break;
	}

	// Get the colour
	// Set the light's colour
	FMVector3 colour = pColladaLight->GetColor();

	pLight->m_colour = RsColour( colour.x, colour.y, colour.z, 1.0f );

	// Get the intensity
	pLight->m_intensity = pColladaLight->GetIntensity();

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// ExtractGeometry

void ExScene::ExtractGeometry( FCDGeometryInstance* pGeometryInstance,
							   FCDGeometry* pGeometry )
{
	// Create a mesh
	m_pCurrentSceneNode->m_pMesh = new ExSgMesh( m_pCurrentSceneNode, this );

	//// Get the mesh
	FCDGeometryMesh* pMesh = pGeometry->GetMesh();

	if( pMesh == BtNull )
	{
		ErrorLog::Fatal_Printf( "Extract geometry failed. Mesh pointer BtNull in %s.\n", GetTitle() );
	}

	// Triangulate each mesh
	FCDGeometryPolygonsTools::Triangulate( pMesh );

	// Cache the number of materials
	BtU32 nMaterialInstanceCount = (BtU32) pGeometryInstance->GetMaterialInstanceCount();

	// Loop through the materials
	for( BtU32 nMaterial = 0; nMaterial<nMaterialInstanceCount; nMaterial++ )
	{
		// Cache each material instance
		FCDMaterialInstance* pMaterialInstance = pGeometryInstance->GetMaterialInstance( nMaterial );

		// Cache the material entity
		FCDMaterial* pMaterialEntity = pMaterialInstance->GetMaterial();

		if( pMaterialEntity == BtNull )
			continue;
		
		FCDGeometryPolygonsList polygonList;

		// Find the polygons for this material
		pMesh->FindPolygonsByMaterial( pMaterialInstance->GetSemantic(), polygonList );

		BtU32 nPolygonCount = (BtU32) polygonList.size();

		if( nPolygonCount > 0 )	
		{
			// Cache the effect
			FCDEffect* pEffect = pMaterialEntity->GetEffect();

			// Validate the effect
			if( pEffect == BtNull )
			{
				ErrorLog::Fatal_Printf( "Material does not have an effect\n" );
			}

			// Cache the effect profile
			for( BtU32 iProfile=FUDaeProfileType::CG; iProfile < FUDaeProfileType::UNKNOWN; ++iProfile )
			{
				FCDEffectProfile* pEffectProfile = pEffect->FindProfile( (FUDaeProfileType::Type)iProfile );

				if( pEffectProfile != BtNull )
				{
					// Set the material name
					BaMaterialFileData material;
					BtChar MaterialName[MaxMaterialName];
					BtStrCopy( MaterialName, MaxMaterialName, GetTitle() );
					BtStrCat( MaterialName, MaxMaterialName, (BtChar*) &pMaterialEntity->GetName()[0] );

					// Create a new material
					ExMaterial* pNewMaterial = new ExMaterial();
			
					// Set the material name
					pNewMaterial->SetName( MaterialName );

					// Cache the effect profile type
					FUDaeProfileType::Type type = pEffectProfile->GetType();

					switch( type )			
					{
						case FUDaeProfileType::CG :
							{
								FCDEffectProfileFX* pFXProfile = dynamic_cast<FCDEffectProfileFX*>(pEffectProfile);
								(void)pFXProfile;
								int a=0;
								a++;
							}
							break;

						case FUDaeProfileType::COMMON :
							{
								ExStandardShader standardShader( pPath(), &m_textureList, pNewMaterial );

								if( standardShader.IsEnvironmentMapped() == BtTrue )
								{
									m_pCurrentSceneNode->SetNodeType( ExSgType_EnvMapped );
								}

								standardShader.Export( pEffectProfile );
							}
							break;

						default:

							pNewMaterial = BtNull;
							break;
					}

					BtAssert( pNewMaterial != BtNull );

					// Find the material
					ExMaterial* pMaterial = m_materialList.pFind( pNewMaterial );

					// Force not to find
					pMaterial = BtNull;

					if( pMaterial == BtNull )
					{
						// Add the material
						m_materialList.Add( pNewMaterial );

						pMaterial = pNewMaterial;
					}
					else
					{
						delete pNewMaterial;
					}

					FCDGeometryPolygonsList polygonList;

					// Find the polygons for this material
					pMesh->FindPolygonsByMaterial( pMaterialInstance->GetSemantic(), polygonList );

					BtU32 iIndex = 0;

					// Create a new material block
					ExMaterialBlock* pMaterialBlock = new ExMaterialBlock;

					pMaterialBlock->m_pMaterial = pMaterial;

					for( BtU32 iPoly=0; iPoly<nPolygonCount; iPoly++ )
					{
						// Cache each polygon
						FCDGeometryPolygons* pPolygon = polygonList.at( iPoly );

						// Get the number of polygon inputs
						BtU32 nPolygonInputs = (BtU32) pPolygon->GetInputCount();

						// Get the vertex count
						BtU32 nVertexCount = (BtU32) pPolygon->GetFaceCount() * 3;

						for( BtU32 iVertex=0; iVertex<nVertexCount; iVertex++ )
						{
							ExVertex* pVertex = new ExVertex;

							BtU32 iUVSet = 0;

							for( BtU32 iInput = 0; iInput<nPolygonInputs; iInput++ )
							{
								const FCDGeometryPolygonsInput* pInput = pPolygon->GetInput(iInput);

								switch( pInput->GetSource()->GetType() )
								{
									case FUDaeGeometryInput::POSITION :
									{
										const BtFloat* pSource = pInput->GetSource()->GetData();
										BtU32 nStride = pInput->GetSource()->GetStride();
										const uint32* pPositionIndices = pInput->GetIndices();

										if( iVertex < pInput->GetIndexCount() )
										{
											// Get the index
											BtU32 nIndex = pPositionIndices[iVertex];

											// Get the position of the data
											BtU32 nPosition = nIndex * nStride;

											// Set the data
											pVertex->Position( pSource[nPosition + 0],
															   pSource[nPosition + 1],
															   pSource[nPosition + 2] );

											if( m_pCurrentSceneNode->GetNodeType( ExSgType_Skin ) == BtTrue )
											{
												// Cache the pairs for this vertex
												const ExVertexJointWeightPairs& pairs = m_pCurrentSceneNode->JointWeightPair( nIndex );

												// Ensure each triangle uses a weight
												BtAssert( pairs.m_nWeightsUsed != 0 );

												// Loop through all the used weights
												for( BtU32 iInfluence=0; iInfluence<pairs.m_nWeightsUsed; iInfluence++ )
												{
													// Cache each pair
													const ExJointWeightPair& pair = pairs.m_jointWeightPair[iInfluence];

													// Set the weight
													pVertex->Bone( iInfluence, pair.m_fNormalisedWeight, pair.m_iBone );
												}
											}
										}
										else
										{
											int a=0;
											a++;
										}
										break;
									}

									case FUDaeGeometryInput::TEXTANGENT :
										{
											const BtFloat* pSource = pInput->GetSource()->GetData();
											BtU32 nStride = pInput->GetSource()->GetStride();
											const uint32* pPositionIndices = pInput->GetIndices();

											// Get the index
											BtU32 nIndex = pPositionIndices[iVertex];

											// Get the position of the data
											BtU32 nPosition = nIndex * nStride;

											// Set the data
											pVertex->Tangent( pSource[nPosition + 0],
															  pSource[nPosition + 1],
															  pSource[nPosition + 2] );
											break;
										}

									case FUDaeGeometryInput::TEXBINORMAL :
										{
											const BtFloat* pSource = pInput->GetSource()->GetData();
											BtU32 nStride = pInput->GetSource()->GetStride();
											const uint32* pPositionIndices = pInput->GetIndices();

											// Get the index
											BtU32 nIndex = pPositionIndices[iVertex];

											// Get the position of the data
											BtU32 nPosition = nIndex * nStride;

											// Set the data
											pVertex->Binormal( pSource[nPosition + 0],
															   pSource[nPosition + 1],
															   pSource[nPosition + 2] );
											break;
										}

									case FUDaeGeometryInput::NORMAL :
										{
											const BtFloat* pNormalSource = pInput->GetSource()->GetData();
											BtU32 nStride = pInput->GetSource()->GetStride();
											const uint32* pPositionIndices = pInput->GetIndices();

											// Get the index
											BtU32 nIndex = pPositionIndices[iVertex];

											// Get the position of the data
											BtU32 nPosition = nIndex * nStride;

											// Set the data
											pVertex->Normal( pNormalSource[nPosition + 0],
															 pNormalSource[nPosition + 1],
															 pNormalSource[nPosition + 2] );
											break;
										}

									case FUDaeGeometryInput::TEXCOORD :
									{
										const BtFloat* pUVSource = pInput->GetSource()->GetData();
										BtU32* pUVIndices = (BtU32*) pInput->GetIndices();

										BtU32 nStride = pInput->GetSource()->GetStride();

										// Get the index
										BtU32 nIndex = pUVIndices[iVertex];

										// Get the position of the data
										BtU32 nPosition = nIndex * nStride;

										BtFloat repeatU = pMaterial->GetRepeatU();
										BtFloat repeatV = pMaterial->GetRepeatV();
											
										BtFloat offsetU = pMaterial->GetOffsetU();
										BtFloat offsetV = pMaterial->GetOffsetV();

										BtFloat ux = pUVSource[nPosition + 0] * repeatU;
										BtFloat uy = pUVSource[nPosition + 1] * repeatV;

										ux += offsetU;
										uy += offsetV;

										// Set the data
										pVertex->UV( iUVSet, ux, uy );

										if( iUVSet == 1 )
										{
											int a=0;
											a++;
										}
										++iUVSet;
										break;
									}
								}
							}

							// Set the colour
							pVertex->Colour( 1.0f, 1.0f, 1.0f, 1.0f );

							// Add each vertex
							pMaterialBlock->m_pVertex.push_back( pVertex );

							if( ( iVertex % 3 ) == 0 )
							{
								// Set the relative index into each group
								pMaterialBlock->m_indices.push_back( iIndex + 0 );
								pMaterialBlock->m_indices.push_back( iIndex + 2 );
								pMaterialBlock->m_indices.push_back( iIndex + 1 );			

								// Increment the group index
								iIndex+=3;
							}
						}
					}

					if( pMaterialBlock->m_pVertex.size() )
					{
						// Cache any vertex
						ExVertex* pVertex = pMaterialBlock->m_pVertex[0];

						// Set the material based on the best vertex format
						pMaterial->VertexType( pVertex->GetVertexType() );

						if( m_pCurrentSceneNode->GetNodeType( ExSgType_Skin ) == BtTrue )
						{
							m_pCurrentSceneNode->m_pSkin->m_materialBlocks.push_back( pMaterialBlock );
						}
						
						if( m_pCurrentSceneNode->GetNodeType( ExSgType_Mesh ) == BtTrue )
						{
							m_pCurrentSceneNode->m_pMesh->m_materialBlocks.push_back( pMaterialBlock );
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// OptimiseGeometry

void ExScene::OptimiseGeometry()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		ExSgNode* pNode = m_pSgNodes[nSceneNode];

		BtChar *name = pNode->m_fileData.m_name;
		(void)name;

		if( pNode->GetNodeType( ExSgType_Mesh ) == BtTrue )
		{
			// Cache each mesh
			ExSgMesh* pMesh = pNode->m_pMesh;

			// Optimise the geometry
			pMesh->GroupDrawing();
		}

		if( pNode->GetNodeType( ExSgType_Skin ) == BtTrue )
		{
			// Cache each skin
			ExSgSkin* pSkin = pNode->m_pSkin;

			// Optimise the geometry
			pSkin->GroupDrawing();
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// ExtractToMaterialAndRenderGroups

void ExScene::ExtractToMaterialAndRenderGroups()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		if( pSceneNode->GetNodeType( ExSgType_Mesh ) )
		{
			// Cache the number of material blocks
			BtU32 nMaterialBlocks  = (BtU32)pSceneNode->m_pMesh->m_materialBlocks.size();

			// Flatten the material blocks
			for( BtU32 nMaterialBlock=0; nMaterialBlock<nMaterialBlocks; nMaterialBlock++ )
			{
				ExMaterialBlock* pMaterialBlock;

				pMaterialBlock = pSceneNode->m_pMesh->m_materialBlocks[nMaterialBlock];

				if( pMaterialBlock->m_pMaterial == BtNull )
				{
					pMaterialBlock->m_fileData.m_nMaterial = 0; 
				}
				else
				{
					pMaterialBlock->m_fileData.m_nMaterial = pMaterialBlock->m_pMaterial->GetResourceID(); 
				}

				pMaterialBlock->m_fileData.m_nStartRenderBlock = (BtU32) m_renderBlocks.size();

				// Flatten the render blocks
				BtU32 nRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

				for( BtU32 iRenderBlock=0; iRenderBlock<nRenderBlocks; iRenderBlock++ )
				{
					// Cache each render block
					ExRenderBlock& renderBlock = pMaterialBlock->m_renderBlocks[iRenderBlock];

					renderBlock.m_fileData.m_nStartPrimitiveBlock = (BtU32) m_primitives.size();

					// Flatten the primitives
					BtU32 nPrimitives = (BtU32) renderBlock.m_primitives.size();

					for( BtU32 nPrimitive=0; nPrimitive<nPrimitives; nPrimitive++ )
					{
						// Add each primitive
						m_primitives.push_back( renderBlock.m_primitives[nPrimitive] );
					}

					renderBlock.m_fileData.m_nEndPrimitiveBlock = (BtU32) m_primitives.size();
					for( BtU32 iBone=0; iBone<MaxBoneMatrices; iBone++ )
					{
						renderBlock.m_fileData.m_bonePalette[iBone] = renderBlock.m_bonePalette[iBone];
					}
					renderBlock.m_fileData.m_bonePaletteSize = renderBlock.m_bonePaletteSize;
					m_renderBlocks.push_back( renderBlock.m_fileData );
				}

				pMaterialBlock->m_fileData.m_nEndRenderBlock = (BtU32) m_renderBlocks.size();
				pSceneNode->m_materialBlocks.push_back( pMaterialBlock->m_fileData );
			}
		}
		
		if( pSceneNode->GetNodeType( ExSgType_Skin ) )
		{
			// Cache the number of material blocks
			BtU32 nMaterialBlocks = (BtU32)pSceneNode->m_pSkin->m_materialBlocks.size();

			// Flatten the material blocks
			for( BtU32 nMaterialBlock=0; nMaterialBlock<nMaterialBlocks; nMaterialBlock++ )
			{
				ExMaterialBlock* pMaterialBlock;

				pMaterialBlock = pSceneNode->m_pSkin->m_materialBlocks[nMaterialBlock];

				if( pMaterialBlock->m_pMaterial == BtNull )
				{
					pMaterialBlock->m_fileData.m_nMaterial = 0; 
				}
				else
				{
					pMaterialBlock->m_fileData.m_nMaterial = pMaterialBlock->m_pMaterial->GetResourceID(); 
				}

				pMaterialBlock->m_fileData.m_nStartRenderBlock = (BtU32) m_renderBlocks.size();

				// Flatten the render blocks
				BtU32 nRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

				for( BtU32 nRenderBlock=0; nRenderBlock<nRenderBlocks; nRenderBlock++ )
				{
					// Cache each render block
					ExRenderBlock& renderBlock = pMaterialBlock->m_renderBlocks[nRenderBlock];

					renderBlock.m_fileData.m_nStartPrimitiveBlock = (BtU32) m_primitives.size();

					// Flatten the primitives
					BtU32 nPrimitives = (BtU32) renderBlock.m_primitives.size();

					for( BtU32 nPrimitive=0; nPrimitive<nPrimitives; nPrimitive++ )
					{
						// Add each primitive
						m_primitives.push_back( renderBlock.m_primitives[nPrimitive] );
					}

					renderBlock.m_fileData.m_nEndPrimitiveBlock = (BtU32) m_primitives.size();
					for( BtU32 iBone=0; iBone<MaxBoneMatrices; iBone++ )
					{
						renderBlock.m_fileData.m_bonePalette[iBone] = renderBlock.m_bonePalette[iBone];
					}
					renderBlock.m_fileData.m_bonePaletteSize = renderBlock.m_bonePaletteSize;
					m_renderBlocks.push_back( renderBlock.m_fileData );
				}

				pMaterialBlock->m_fileData.m_nEndRenderBlock = (BtU32) m_renderBlocks.size();
				pSceneNode->m_materialBlocks.push_back( pMaterialBlock->m_fileData );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MarkUpHierarchy

void ExScene::MarkUpHierarchy()
{
	// Get the scene node base resource id
	BtU32 nSceneNodeBaseResourceID = PaPacker::GlobalID();

	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32)m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		// Set the scene resource ID
		pSceneNode->m_fileData.m_nScene = m_nSceneResourceID;

		// Set the resource IDs
		pSceneNode->SetResourceID( nSceneNodeBaseResourceID + nSceneNode );

		int a = 0;
		a++;
	}

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		// Mark up the hierarchy
		pSceneNode->MarkUpHierarchy();
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExportVertexBuffer

void ExScene::ExportVertexBuffer()
{
	for( BtU32 i=0; i<MaxVertexDeclarations; i++ )
	{
		m_vertexBuffer[i].SetStatic();
		m_vertexBuffer[i].Export();
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExportCollision

void ExScene::ExportCollision()
{
	m_collisionMesh.Export();
	m_collisionShapes.Export();
}

////////////////////////////////////////////////////////////////////////////////
// FlattenAnimation

void ExScene::FlattenAnimation()
{
	BtFloat frameNumber = 0;

	BtU32 maxFrames = (BtU32)( m_maxAnimationDuration * m_animationFrameRate );

	BtFloat frameTime = 1.0f / m_animationFrameRate;
	BtFloat sampleTime = 0;

	while( frameNumber < maxFrames )
	{
		BaSgAnimationTimeFileData time;
		time.m_time = sampleTime;
		time.m_iTransformStart = (BtU32) m_animationTransforms.size();

		// Cache the number of scene nodes
		BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

		// Loop through the scene nodes
		for( BtU32 iSceneNode=0; iSceneNode<nSceneNodes; iSceneNode++ )
		{
			// Cache each node
			ExSgNode* pNode = m_pSgNodes[iSceneNode];

			// Cache each animation
			const ExSgAnimation* pAnimation = pNode->m_pAnimation;

			if( pAnimation != BtNull )
			{
				// Add a full local transform for the first time
				BaSgAnimationTransformFileData transform;
				
				if (pNode->m_pBone == BtNull)
				{
					transform.m_nodeIndex = iSceneNode;
					transform.m_iBone = -1;
				}
				else
				{
					transform.m_nodeIndex = -1;
					transform.m_iBone = pNode->m_pBone->BoneIndex();
				}

				//BtAssert( pNode->m_pBone != BtNull );

				transform.m_m4Transform = pNode->Local();
				m_animationTransforms.push_back( transform );

				// Cache each animation
				const ExSgAnimation* pAnimation = pNode->m_pAnimation;

				// Cache the number of animation keys
				BtU32 nAnimationKeys = (BtU32) pAnimation->AnimationKeys().size();

				// Loop through the keys
				for( BtU32 iKey=0; iKey<nAnimationKeys; iKey++ )
				{
					// Cache each key
					const ExSgAnimationKey& key = pAnimation->AnimationKeys()[iKey];

                    // find the animation key for this time
					if( iKey == frameNumber )
					{
						transform.m_m4Transform = key.m_m4Transform;
						ExMatrix::ChangeCoordinateSystem( transform.m_m4Transform );
						m_animationTransforms.push_back( transform );
						break;
					}
				}
			}
		}

		time.m_iTransformEnd = (BtU32) m_animationTransforms.size();
		m_animationTimes.push_back( time );

		// Increment the time we're using to slot animation keys
		frameNumber++;

		// Increment the time
		sampleTime += frameTime;
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// ExportAnimation

void ExScene::ExportAnimation()
{
	if( m_animationTimes.size() )
	{
		// Write out the scene
		PaMemoryResource sceneAnimation( PMR_Write );

		// Cache the number of animation nodes
		BtU32 maxFrames = (BtU32) m_animationTimes.size();

		BaSgAnimationFileData animation;
		animation.m_duration  = (BtU32)m_maxAnimationDuration;
		animation.m_maxFrames = maxFrames;
		animation.m_frameRate = m_animationFrameRate;
		sceneAnimation << animation;

		// Loop through the animation times
		for( BtU32 frames=0; frames<maxFrames; frames++ )
		{
			// Cache each time
			BaSgAnimationTimeFileData& time = m_animationTimes[frames];

			// Serialise the block
			sceneAnimation << time;
		}

		// Cache the number of animation transforms
		BtU32 nAnimationTransforms = (BtU32) m_animationTransforms.size();

		// Loop through the transforms
		for( BtU32 iTransform=0; iTransform<nAnimationTransforms; iTransform++ )
		{
			// Cache each transform
			BaSgAnimationTransformFileData& transform = m_animationTransforms[iTransform];

			if (transform.m_iBone == -1)
			{
				// Cache the scene node
				ExSgNode *pNode = m_pSgNodes[transform.m_nodeIndex];

				// Get the resource ID
				transform.m_nodeIndex = pNode->GetResourceID();
			}

			// Serialise the transforms
			sceneAnimation << transform;
		}

		// Add the resource
		AddResource( sceneAnimation,
					 BtNull,
					 GetTitle(),
					 BaRT_Animator );

		ErrorLog::Printf( "Animation size %d KBytes\r\n", sceneAnimation.Tell() );
	}
}

////////////////////////////////////////////////////////////////////////////////
// SerialiseSceneNodes

void ExScene::SerialiseSceneNodes()
{
	// Write out the scene
	PaMemoryResource sceneResource( PMR_Write );

	// Cache the number of scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	// Loop through the scene nodes
	for( BtU32 iSceneNode=0; iSceneNode<nSceneNodes; iSceneNode++ )
	{
		// Copy the attributes
		m_pSgNodes[iSceneNode]->CopyAttributes();

		// Export each scene node
		m_pSgNodes[iSceneNode]->Export();
	};
}

////////////////////////////////////////////////////////////////////////////////
// ExportIndexBuffer

void ExScene::ExportIndexBuffer()
{
	m_indexBuffer.Export();
}

////////////////////////////////////////////////////////////////////////////////
// ExportResources

void ExScene::ExportResources()
{
	FlattenAnimation();

	ExportVertexBuffer();
	ExportIndexBuffer();

	m_materialList.Export();

	// Flatten the geometry
	ExtractToMaterialAndRenderGroups();

	// Export physics nodes
	ExportPhysicsNodes();

	// Export the collision
	ExportCollision();

	// Write out the scene
	PaMemoryResource sceneResource( PMR_Write );

	BaSceneFileData scene;
	scene.m_nRenderBlocks = (BtU32) m_renderBlocks.size();;
	scene.m_nPrimitives = (BtU32) m_primitives.size();
	scene.m_nIndexBuffer = m_indexBuffer.GetResourceID();
	scene.m_nCollisionMesh = m_collisionMesh.GetResourceID();
	scene.m_nCollisionAnalytical = m_collisionShapes.GetResourceID();

	for( BtU32 i=0; i<MaxVertexDeclarations; i++ )
	{
		scene.m_nVertexBuffer[i] = m_vertexBuffer[i].GetResourceID();
	}

	sceneResource << scene;

	// Cache the number of render blocks
	BtU32 nRenderBlocks = (BtU32) m_renderBlocks.size();

	// Loop through the Render blocks
	for( BtU32 nRenderBlock=0; nRenderBlock<nRenderBlocks; nRenderBlock++ )
	{
		// Cache each Render block
		const BaRenderBlockFileData& renderBlock = m_renderBlocks[nRenderBlock];
		sceneResource << renderBlock;
	}

	// Cache the number of primitive blocks
	BtU32 nPrimitives = (BtU32) m_primitives.size();

	// Loop through the material blocks
	for( BtU32 nPrimitive=0; nPrimitive<nPrimitives; nPrimitive++ )
	{
		RsIndexedPrimitive primitive = m_primitives[nPrimitive];

		// Set the index size now we know
		primitive.m_indexType = m_indexBuffer.GetIndexType();

		if( primitive.m_indexType == RsIndexBuffer::IndType_Short )
		{
			int a = 0;
			a++;
		}
		else
		{
			int a = 0;
			a++;
		}
		sceneResource << primitive;
	}

	// Add the render block
	m_nSceneResourceID = AddResource( sceneResource, GetFilename(), GetTitle(), BaRT_Scene );

	MarkUpHierarchy();

	// Update the nodes. Calculate world matrices from local matrices
	Update();

	// Export the scene nodes
	SerialiseSceneNodes();

	// Export the animation last
	ExportAnimation();
}

////////////////////////////////////////////////////////////////////////////////
// SetCollisionMeshes

void ExScene::SetCollisionMeshes()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

        BtChar FS = FsFile::GetFileSeparator();
        
		// Set the rigid body filename
		BtChar collisionMeshFilename[MaxFilePath];
		sprintf( collisionMeshFilename, "%s%c%s.cm", pPath(), FS, pSceneNode->pName() );

		// If the rigid body file exists then set the flag
		if( pArchive()->GetFile( collisionMeshFilename ) != BtNull )
		{
			pSceneNode->SetNodeType( ExSgType_Collision );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExScene::ChangeCoordinateSystem()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each node
		ExSgNode* pNode = m_pSgNodes[nSceneNode];

		// Change the coordinate system for the scene node
		pNode->ChangeCoordinateSystem();

		if( pNode->GetNodeType( ExSgType_Mesh ) )
		{
			pNode->m_pMesh->ChangeCoordinateSystem();
		}

		if( pNode->GetNodeType( ExSgType_Light ) )
		{
			pNode->m_pLight->ChangeCoordinateSystem();
		}

		if( pNode->GetNodeType( ExSgType_Bone ) )
		{
			pNode->m_pBone->ChangeCoordinateSystem();
		}

		if( pNode->GetNodeType( ExSgType_Skin ) )
		{
			pNode->m_pSkin->ChangeCoordinateSystem();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExtractDocument

void ExScene::ExtractDocument()
{
	FUErrorSimpleHandler errorHandler;

	if( BtStrCompare( GetExtension(), "dae" ) == BtTrue )
	{
		// Make a new document
		m_pDocument = FCollada::NewTopDocument();
		
		// Load the document
		FCollada::LoadDocumentFromFile( m_pDocument, GetFilename() );

		BtChar author[256];
		strcpy( author, m_pDocument->GetAsset()->GetContributor(0)->GetAuthor() );
		strlower( author );

		if( strstr( author, "blender" ) )
		{
			m_isBlender = BtTrue;
		}
		else
		{
			m_isBlender = BtFalse;
		}

		m_isMetasequoia = BtFalse;
		
		if( errorHandler.IsSuccessful() == BtFalse )
		{
			ErrorLog::Fatal_Printf( "Failed to load Collada document %s\n", GetFilename() );
		}
	}
	else
	{
		m_isMetasequoia = BtTrue;

		MCMetasequoia metasequoia;
		metasequoia.Load( GetFilename() );

		// make a temporary folder
        FsFile::CreateFolder( "\\temp" );

		m_pColladaWrapper = new MCCollada();
		m_pColladaWrapper->Create( metasequoia.GetVanilla() );
		m_pColladaWrapper->Export( "\\temp\\test.dae" );
		//m_pDocument = m_pColladaWrapper->GetDocument();

		m_pDocument = FCollada::NewTopDocument();
		FCollada::LoadDocumentFromFile( m_pDocument, "\\temp\\test.dae" );

		if( errorHandler.IsSuccessful() == BtFalse )
		{
			ErrorLog::Fatal_Printf( "Failed to load Collada document %s\n", GetFilename() );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// CopyBlendShapes

void ExScene::CopyBlendShapes()
{
	// Cache number the bones
	BtU32 nSize = (BtU32) m_pSgNodes.size();

	// Index the bones
	for( BtU32 iNode=0; iNode<nSize; iNode++ )
	{
		// Cache each node
		ExSgNode* pSceneNode = m_pSgNodes[iNode];

		// Copy the meshes
		if( pSceneNode->GetNodeType( ExSgType_TargetBlendShape ) )
		{
			pSceneNode->m_pBlendShape->CopyBlendShapes();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MoveToVertexBuffers

void ExScene::MoveToVertexBuffers()
{
	// Cache number the bones
	BtU32 nSize = (BtU32) m_pSgNodes.size();

	// Index the bones
	for( BtU32 iNode=0; iNode<nSize; iNode++ )
	{
		// Cache each node
		ExSgNode* pSceneNode = m_pSgNodes[iNode];

		// Copy the meshes
		if( pSceneNode->GetNodeType( ExSgType_Skin ) == BtTrue )
		{
			pSceneNode->m_pSkin->MoveToVertexBuffers();
		}

		if( pSceneNode->GetNodeType( ExSgType_Mesh ) == BtTrue )
		{
			pSceneNode->m_pMesh->MoveToVertexBuffers();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExScene::Export()
{
	// Get the document
	ExtractDocument();

	// Cache the visual scene
	FCDSceneNode* pVisualScene = m_pDocument->GetVisualSceneInstance();

	// Extract the hierarchy
	ExtractHierarchy( pVisualScene, BtNull );

	// Setup the hierarchy
	SetExportHierarchy();

	// Get the max animation length
	GetAnimationLength();

	// Extract the scene nodes
	ExtractSceneNodes();

	// Extract the hinges
	ExtractHinges();

	// Extract the physics nodes
	ExtractPhysicsNodes();

	// Change the coordinate system
	ChangeCoordinateSystem();

	// Set collision meshes
	SetCollisionMeshes();

	// Optimize the geometry
	OptimiseGeometry();

	// Copy the blend shapes
	CopyBlendShapes();

	// Move to vertex buffers
	MoveToVertexBuffers();

	// Export resources
	ExportResources();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void ExScene::Destroy()
{
	// Loop through the scene nodes
	BtU32 nSceneNodes = (BtU32) m_pSgNodes.size();

	for( BtU32 nSceneNode=0; nSceneNode<nSceneNodes; nSceneNode++ )
	{
		// Cache each scene node
		ExSgNode* pSceneNode = m_pSgNodes[nSceneNode];

		// Delete each scene node
		delete pSceneNode;
	}

	// Destroy the document
	if( BtStrCompare( GetExtension(), "dae" ) == BtTrue )
	{
		if( m_pDocument != BtNull )
		{
			m_pDocument->Release();
		}
	}
	else
	{
		m_pColladaWrapper->Destroy();
		delete m_pColladaWrapper;

		if( m_pDocument != BtNull )
		{
			m_pDocument->Release();
		}
	}
}

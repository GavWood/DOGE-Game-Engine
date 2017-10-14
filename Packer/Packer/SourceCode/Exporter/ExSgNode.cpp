////////////////////////////////////////////////////////////////////////////////
// ExSgNode.cpp

// Includes
#include "StdAfx.h"
#include "FCollada.h"
#include "ExMatrix.h"
#include "ExSgNode.h"
#include "ExSgMesh.h"
#include "ExSgBone.h"
#include "ExSgSkin.h"
#include "ExSgAnimation.h"
#include "ExRigidBody.h"
#include "ExSgCamera.h"
#include "ExSgBlendShape.h"
#include "ExSgLight.h"

#include "PaTopState.h"
#include "PaRendering.h"

#include "FCDSceneNode.h"
#include "FCDSceneNodeTools.h"
#include "FCDocument.h"
#include "FCDAsset.h"
#include "FCDLibrary.h"
#include "FCDController.h"
#include "FCDSkinController.h"
#include "FCDGeometry.h"
#include "FCDGeometryInstance.h"
#include "FCDGeometryMesh.h"
#include "FCDGeometryPolygons.h"
#include "FCDGeometryPolygonsTools.h"
#include "FCDGeometrySource.h"
#include "FCDSceneNode.h"
#include "FCDTransform.h"
#include "FCDMaterialInstance.h"
#include "FCDMaterial.h"
#include "FCDEffect.h"
#include "FCDEffectStandard.h"
#include "FCDTexture.h"
#include "FCDImage.h"
#include "FUFileManager.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgNode::ExSgNode( ExScene* pScene ) : m_pCollision( NULL )
{
	m_nodeType = 0;
	m_pParent = BtNull;
	m_pFirstChild = BtNull;
	m_pNextSibling = BtNull;
	m_jointWeightPairs.clear();
	m_pScene = pScene;
	m_pRigidBody = BtNull;
	m_pBlendShape = BtNull;
	m_pBone = BtNull;
	m_pMesh = BtNull;
	m_pSkin = BtNull;
	m_pCamera = BtNull;
	m_pAnimation = BtNull;
	m_pLight = BtNull;
	m_m4Local.SetIdentity();
	m_m4World.SetIdentity();
	m_pRigidBodyInstance = BtNull;
	BtMemory::Set( m_id, 0, sizeof(m_id) );
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgNode::~ExSgNode()
{
	if( m_pRigidBody != NULL )
	{
		delete m_pRigidBody;
	}

	if( m_pCollision != NULL )
	{
		delete m_pCollision;
	}

	if( m_pBone != NULL )
	{
		delete m_pBone;
	}

	if( m_pCamera != NULL )
	{
		delete m_pCamera;
	}

	if( m_pMesh != NULL )
	{
		delete m_pMesh;
	}

	if( m_pSkin != NULL )
	{
		delete m_pSkin;
	}

	if( m_pAnimation != NULL )
	{
		delete m_pAnimation;
	}

	if( m_pBlendShape != NULL )
	{
		delete m_pBlendShape;
	}

	if( m_pLight != NULL )
	{
		delete m_pLight;
	}
}


////////////////////////////////////////////////////////////////////////////////
// isStripped

BtBool ExSgNode::isStripped()
{
//	if( BtStrStr( m_fileData.m_name, "Heightmap" ) )
//	{
//		return BtFalse;
//	}

	if( GetNodeType( ExSgType_RigidBody ) )
	{
		return BtFalse;
	}
	return BtFalse;

	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgNode::ChangeCoordinateSystem()
{
	ExMatrix::ChangeCoordinateSystem( m_m4Local );
	ExMatrix::ChangeCoordinateSystem( m_m4World );
}

////////////////////////////////////////////////////////////////////////////////
// ExtractJointWeightPairs

void ExSgNode::ExtractJointWeightPairs( const FCDEntityInstance* pEntityInstance )
{
	// Get the controller
	const FCDController* pController = (FCDController*) pEntityInstance->GetEntity();

	if( pController->IsSkin() != BtTrue )
	{
		return;
	}

	// Cache the skin controller
	const FCDSkinController* pSkinController = (FCDSkinController*) pController->GetSkinController();

    // Cache the vertex influence count
	BtU32 nVertexInfluenceCount = (BtU32) pSkinController->GetInfluenceCount();

	// Copy the joint weight pairs across
	for( BtU32 iInfluence=0; iInfluence<nVertexInfluenceCount; iInfluence++ )
	{
		// Get the vertex influences
		const FCDSkinControllerVertex* pVertex = pSkinController->GetVertexInfluence( iInfluence );

		// Get the pair count
		BtU32 nJointWeightPairs = (BtU32) pVertex->GetPairCount();

		FCDJointWeightPair UnsortedPairs[256];

		// Sort the joint weight pairs
		for( BtU32 iJointWeightPair1=0; iJointWeightPair1<nJointWeightPairs; iJointWeightPair1++ )
		{
			const FCDJointWeightPair* pJointWeightPair1 = pVertex->GetPair( iJointWeightPair1 );

			UnsortedPairs[iJointWeightPair1] = *pJointWeightPair1;
		}

		// Sort the joint weight pairs
		for( BtU32 iJointWeightPair1=0; iJointWeightPair1<nJointWeightPairs; iJointWeightPair1++ )
		{
            for( BtU32 iJointWeightPair2=0; iJointWeightPair2<nJointWeightPairs; iJointWeightPair2++ )
			{
				if( UnsortedPairs[iJointWeightPair1].weight > UnsortedPairs[iJointWeightPair2].weight )
				{
					BtSwap( UnsortedPairs[iJointWeightPair1], UnsortedPairs[iJointWeightPair2] );
				}
			}
		}

		// Get the number of weights used
		BtU32 WeightsUsed = MtMin( MaxBoneInfluence, nJointWeightPairs );

		// Setup the array of joint weight pairs for this vertex
		ExVertexJointWeightPairs vertexJointWeightPair;

		for( BtU32 iJointWeightPair=0; iJointWeightPair<WeightsUsed; iJointWeightPair++ )
		{
			// Cache each joint weight pair
			ExJointWeightPair& jointWeightPair = vertexJointWeightPair.m_jointWeightPair[iJointWeightPair];

			// Set the pair of weights and joints
			jointWeightPair.m_fWeight = UnsortedPairs[iJointWeightPair].weight;

			// Cache the joint index
			BtU32 iJoint = UnsortedPairs[iJointWeightPair].jointIndex;

			// Validate the number of joints
			BtAssert( iJoint < MaxBones );

			// Map the joint to the bone index
			jointWeightPair.m_iBone = iJoint;//m_pSkin->BoneMap( iJoint );

			int a=0;
			a++;
		}

		// Normalise the joint weight pairs
		vertexJointWeightPair.m_nWeightsUsed = WeightsUsed;
		vertexJointWeightPair.Normalise();

		// Add the joint weight pair
		m_jointWeightPairs.push_back( vertexJointWeightPair );
	}

	int a=0;
	a++;
}


////////////////////////////////////////////////////////////////////////////////
// GetAnimationLength

BtFloat ExSgNode::GetAnimationLength()
{
	// Clear up the previous animation
	FCDSceneNodeTools::ClearSampledAnimation();

	// Generate a list of matrices, with corresponding key times that represent an animation curve for the local transform
	// of a scene node. 
	FCDSceneNodeTools::GenerateSampledAnimation( m_pColladaNode );

	// Get the new keys
	const FloatList& keyTimes = FCDSceneNodeTools::GetSampledAnimationKeys();

	// Cache the number of transforms
	BtU32 numSamples = (BtU32) keyTimes.size();

	if( numSamples == 0 )
	{
		return 0;
	}

	// Return the last time
	return keyTimes[numSamples-1];
}

////////////////////////////////////////////////////////////////////////////////
// ExtractAnimation

void ExSgNode::ExtractAnimation( BtFloat maxAnimationDuration, BtFloat frameRate )
{
	// Clear up the previous animation
	FCDSceneNodeTools::ClearSampledAnimation();

	// Generate a list of matrices, with corresponding key times that represent an animation curve for the local transform
	// of a scene node. 
	FCDSceneNodeTools::GenerateSampledAnimation( maxAnimationDuration, frameRate, m_pColladaNode );

	// Get the sample values
	const FMMatrix44List& keyTransforms = FCDSceneNodeTools::GetSampledAnimationMatrices();

	// Cache the number of transforms
	BtU32 numSamples = (BtU32) keyTransforms.size();

	// Exit if there are no transforms extracted
	if( numSamples == 0 )
	{
		return;
	}

	// Create an animation
	m_pAnimation = new ExSgAnimation( this, m_pScene );

	BtFloat keyTime = 0;

	// Loop through the transforms
	for( BtU32 sampleIndex=0; sampleIndex<numSamples; sampleIndex++ )
	{
		// Cache each transform
		const FMMatrix44* pTransform = &keyTransforms[sampleIndex];

		// Setup the animation block
		ExSgAnimationKey key;
		key.m_time = keyTime;
		BtMemory::Copy( &key.m_m4Transform, pTransform, sizeof(MtMatrix4) );

		keyTime += 1.0f / frameRate;

		// Add the animation block
		m_pAnimation->AddAnimation( key );
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetExportHierarchy

void ExSgNode::SetExportHierarchy()
{
	// Cache the number of children
	BtU32 nChildren = (BtU32) m_children.size();

	// Set the first child
	if( nChildren > 0 )
	{
		// Set the first child
		m_pFirstChild = m_children[0];

		// Loop through the children
		for( BtU32 nChild=0; nChild<nChildren - 1; nChild++ )
		{
			ExSgNode* pFirst  = m_children[nChild];
			ExSgNode* pSecond = m_children[nChild + 1];

			pFirst->m_pNextSibling = pSecond;
		}

		// Recurse through the children
		for( BtU32 nChild = 0; nChild<nChildren; nChild++ )
		{
			// Cache each child
			ExSgNode* pChild = m_children[nChild];

			// Set the parent of each child
			pChild->m_pParent = this;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MarkUpHierarchy

void ExSgNode::MarkUpHierarchy()
{
	m_fileData.m_renderFlags = RsRF_Visible;

	// Set the resource IDs
	if( m_pParent != BtNull )
	{
		m_fileData.m_nParent = m_pParent->m_nResourceID;
	}
	else
	{
		m_fileData.m_nParent = BtNull;
	}

	if( m_pFirstChild != BtNull )
	{
		m_fileData.m_nFirstChild = m_pFirstChild->m_nResourceID;
	}
	else
	{
		m_fileData.m_nFirstChild = BtNull;
	}

	if( m_pNextSibling != BtNull )
	{
		m_fileData.m_nNextSibling = m_pNextSibling->m_nResourceID;
	}
	else
	{
		m_fileData.m_nNextSibling = BtNull;
	}

	// Set the number of children
	m_fileData.m_nChildren = (BtU32) m_children.size();
}

////////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgNode::CopyAttributes()
{
	// Set the game node type
	BtU32 nodeType = 0;

	if( GetNodeType( ExSgType_Mesh ) )
	{
		nodeType |= SgType_Mesh;
	}
	if( GetNodeType( ExSgType_Skin ) )
	{
		nodeType |= SgType_Skin;
	}
	if( GetNodeType( ExSgType_RigidBody ) )
	{
		nodeType |= SgType_RigidBody;
	}
	if( GetNodeType( ExSgType_Collision ) )
	{
		nodeType |= SgType_Collision;
	}
	if( GetNodeType( ExSgType_Bone ) )
	{
		nodeType |= SgType_Bone;
	}
	if( GetNodeType( ExSgType_EnvMapped ) )
	{
		nodeType |= SgType_EnvMapped;
	}
	if( GetNodeType( ExSgType_Camera ) )
	{
		nodeType |= SgType_Camera;
	}
	if( GetNodeType( ExSgType_TargetBlendShape ) )
	{
		nodeType |= SgType_BlendShape;
	}
	if( GetNodeType( ExSgType_Light ) )
	{
		nodeType |= SgType_Light;
	}
	if( GetNodeType( ExSgType_Mesh ) || GetNodeType( ExSgType_Skin ) )
	{
		nodeType |= SgType_Materials;
	}

	m_fileData.m_nodeType = nodeType;

	BtU32 nFileDataSize = sizeof(BaSgNodeFileData);

	if( GetNodeType( ExSgType_Mesh ) )
	{
		nFileDataSize+=sizeof(BaSgMeshFileData);
	}
	if( GetNodeType( ExSgType_Skin ) )
	{
		nFileDataSize+=sizeof(BaSgSkinFileData);
	}
	if( GetNodeType( ExSgType_RigidBody ) )
	{
		nFileDataSize+=sizeof(BaSgRigidBodyFileData);
	}
	if( GetNodeType( ExSgType_Collision ) )
	{
		nFileDataSize+=sizeof(BaSgCollisionFileData);
	}
	if( GetNodeType( ExSgType_Bone ) )
	{
		nFileDataSize+=sizeof(BaSgBoneFileData);
	}	
	if( GetNodeType( ExSgType_Camera ) )
	{
		nFileDataSize+=sizeof(BaSgCameraFileData);
	}
	if( GetNodeType( ExSgType_TargetBlendShape ) )
	{
		nFileDataSize+=sizeof(BaSgBlendShapeFileData);
	}
	if( GetNodeType( ExSgType_Light ) )
	{
		nFileDataSize+=sizeof(BaSgLightFileData);
	}
	if( GetNodeType( ExSgType_Mesh ) || GetNodeType( ExSgType_Skin) )
	{
		nFileDataSize+=sizeof(BaMaterialsFileData);
		nFileDataSize+=( (BtU32) m_materialBlocks.size() ) * sizeof( BaMaterialBlockFileData );
	}

	// Copy the run time sizes
	m_fileData.m_nFileDataSize = nFileDataSize;

	// Copy the transforms
	m_fileData.m_m4World = m_m4World;
	m_fileData.m_m4Local = m_m4Local;

	// Copy type specific attributes
	if( GetNodeType( ExSgType_Mesh ) )
	{
		m_pMesh->CopyAttributes();
	}

	if( GetNodeType( ExSgType_Camera ) )
	{
		m_pCamera->CopyAttributes();
	}

	if( GetNodeType( ExSgType_Skin ) )
	{
		m_pSkin->CopyAttributes();
	}

	if( GetNodeType( ExSgType_Bone ) )
	{
		m_pBone->CopyAttributes();
	}

	if( GetNodeType( ExSgType_TargetBlendShape ) )
	{
		m_pBlendShape->CopyAttributes();
	}

	if( GetNodeType( ExSgType_RigidBody ) )
	{
		m_pRigidBody->CopyAttributes();
	}

	if( GetNodeType( ExSgType_Light ) )
	{
		m_pLight->CopyAttributes();
	}

	if( GetNodeType( ExSgType_Mesh ) || GetNodeType( ExSgType_Skin ) )
	{
		m_materialsFileData.m_nMaterials = (BtU32) m_materialBlocks.size();
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ExSgNode::Update()
{
	if( m_pParent == BtNull )
	{
		m_m4World = m_m4Local;
	}
	else
	{
		m_m4World = m_m4Local * m_pParent->m_m4World;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExSgNode::Export()
{
	// Create the resource
	PaMemoryResource resource( PMR_Write );

	// Serialise the scene node
	resource << m_fileData;

	// Serialise the mesh file data
	if( GetNodeType( ExSgType_Mesh ) )
	{
		resource << m_meshFileData;
	}

	// Serialise the skin file data
	if( GetNodeType( ExSgType_Skin ) )
	{
		resource << m_skinFileData;
	}

	// Serialise the rigid body
	if( GetNodeType( ExSgType_RigidBody ) )
	{
		resource << m_rigidBodyFileData;
	}

	if( GetNodeType( ExSgType_Collision ) )
	{
		resource << m_collisionFileData;
	}

	if( GetNodeType( ExSgType_Bone ) )
	{
		resource << m_boneFileData;
	}

	if( GetNodeType( ExSgType_Camera ) )
	{
		resource << m_cameraFileData;
	}

	if( GetNodeType( ExSgType_TargetBlendShape ) )
	{
		resource << m_blendShapeFileData;
	}

	if( GetNodeType( ExSgType_Light ) )
	{
		resource << m_lightFileData;
	}

	if( GetNodeType( ExSgType_Mesh ) || GetNodeType( ExSgType_Skin ) )
	{
		resource << m_materialsFileData;

		BtU32 numMaterials = (BtU32) m_materialBlocks.size();

		for( BtU32 iMaterial=0; iMaterial<numMaterials; iMaterial++ )
		{
			resource << m_materialBlocks[iMaterial];
		}
	}

	// Add the resource without a filename so no dependency is made

	if( strstr( m_fileData.m_name, "obj45" ) )
	{
		int a=0;
		a++;
	}

	AddResource( resource,
				 BtNull,
				 m_fileData.m_name, BaRT_SgNode);
//	ErrorLog::Printf( "Added resource %d\r\r", nResourceID() );
}

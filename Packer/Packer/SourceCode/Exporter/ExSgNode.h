////////////////////////////////////////////////////////////////////////////////
// ExSgNode.h

#ifndef __ExSgNode_h__
#define __ExSgNode_h__

#include "BaArchive.h"
#include "BtTypes.h"
#include "SgNode.h"
#include "ExSgAnimation.h"
#include "ExSgCollision.h"
#include "ExMaterialList.h"
#include "ExVertex.h"
#include "ExMaterialBlock.h"
#include "ExVertexJointWeightPairs.h"
#include <vector>

class FCDSceneNode;
class FCDEntityInstance;
class ExScene;
class ExRigidBody;
class ExSgMesh;
class ExSgSkin;
class ExSgLight;
class ExSgBone;
class ExSgCamera;
class ExSgBlendShape;
class FCDPhysicsRigidBodyInstance;

enum ExSgType
{
	ExSgType_Node = 1 << 0,
	ExSgType_Mesh = 1 << 1,
	ExSgType_RigidBody = 1 << 2,
	ExSgType_Skin = 1 << 3,
	ExSgType_Collision = 1 << 4,
	ExSgType_Bone = 1 << 5,
	ExSgType_Camera = 1 << 6,
	ExSgType_TargetBlendShape = 1 << 7,
	ExSgType_SourceBlendShape = 1 << 8,
	ExSgType_Light = 1 << 9,
	ExSgType_EnvMapped = 1 << 10,
	ExSgType_Static = 1 << 11
};

struct ExSgNode : public ExResource
{
	ExSgNode( ExScene *pScene );
	~ExSgNode();

	virtual void							ChangeCoordinateSystem();
	void									MarkUpHierarchy();
	void									SetExportHierarchy();
	void									ExtractJointWeightPairs( const FCDEntityInstance* pEntityInstance );
	BtFloat									GetAnimationLength();
	void									ExtractAnimation( BtFloat maxDuration, BtFloat frameRate );
	void									Update();
	virtual void							Export();
	void									CopyAttributes();

	BtBool									isStripped();
	BtBool									isMergeLikeVertex();
	BtBool									isEnvmapped();

	// Accessors
	BtBool									GetNodeType( ExSgType NodeType);
	void									SetNodeType( ExSgType NodeType );

	BtChar*									pName();
	void									pName( const BtChar* pName );

	void									Local( const MtMatrix4& m4Local );
	const MtMatrix4&						Local() const;
	const MtMatrix4&						World() const;

	const ExVertexJointWeightPairs&			JointWeightPair( BtU32 iVertexIndex );

	// Member variables
	BtChar									m_id[MaxSceneNodeName];

	FCDSceneNode*							m_pColladaNode;

	FCDPhysicsRigidBodyInstance*			m_pRigidBodyInstance;

	std::vector<ExSgNode*>					m_children;
	
	ExSgNode*								m_pParent;

	ExSgNode*								m_pFirstChild;

	ExSgNode*								m_pNextSibling;

	// Instances
	ExRigidBody*							m_pRigidBody;

	ExSgCollision*							m_pCollision;

	ExSgBone*								m_pBone;

	ExSgMesh*								m_pMesh;

	ExSgCamera*								m_pCamera;

	ExSgLight*								m_pLight;

	ExSgSkin*								m_pSkin;

	ExSgAnimation*							m_pAnimation;

	ExSgBlendShape*							m_pBlendShape;

	BaSgNodeFileData						m_fileData;

	BaSgRigidBodyFileData					m_rigidBodyFileData;
	BaSgCollisionFileData					m_collisionFileData;
	BaSgMeshFileData						m_meshFileData;
	BaSgSkinFileData						m_skinFileData;
	BaSgBoneFileData						m_boneFileData;
	BaSgCameraFileData						m_cameraFileData;
	BaSgBlendShapeFileData					m_blendShapeFileData;
	BaSgLightFileData						m_lightFileData;
	BaMaterialsFileData						m_materialsFileData;
	std::vector<BaMaterialBlockFileData>	m_materialBlocks;

	BtU32									m_lastKey;

protected:

	BtU32									m_nodeType;
	std::vector<ExVertexJointWeightPairs>	m_jointWeightPairs;
	BtU32									m_nNodeSize;
	ExScene*								m_pScene;
	MtMatrix4								m_m4Local;
	MtMatrix4								m_m4World;
	BtBool									m_hasEnvmap;
};

////////////////////////////////////////////////////////////////////////////////
// pJointWeightPair

inline const ExVertexJointWeightPairs& ExSgNode::JointWeightPair( BtU32 iVertexIndex )
{
	BtAssert( iVertexIndex < m_jointWeightPairs.size() );
	return m_jointWeightPairs[iVertexIndex];
}

////////////////////////////////////////////////////////////////////////////////
// Local

inline void ExSgNode::Local( const MtMatrix4& m4Local )
{
	m_m4Local = m4Local;
}

inline const MtMatrix4& ExSgNode::Local() const
{
	return m_m4Local;
}

inline const MtMatrix4& ExSgNode::World() const
{
	return m_m4World;
}

////////////////////////////////////////////////////////////////////////////////
// NodeType

inline void ExSgNode::SetNodeType( ExSgType NodeType )
{
	m_nodeType = m_nodeType | NodeType;
}

////////////////////////////////////////////////////////////////////////////////
// NodeType

inline BtBool ExSgNode::GetNodeType( ExSgType NodeType )
{
	if( m_nodeType & NodeType )
	{
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// pName

inline BtChar* ExSgNode::pName()
{
	return m_fileData.m_name;
}

////////////////////////////////////////////////////////////////////////////////
// pName

inline void ExSgNode::pName( const BtChar* pName )
{
	BtStrCopy( m_fileData.m_name, MaxSceneNodeName, pName );
}

////////////////////////////////////////////////////////////////////////////////
// isMergeLikeVertex

inline BtBool ExSgNode::isMergeLikeVertex()
{
	if( GetNodeType( ExSgType_TargetBlendShape ) ||
		GetNodeType( ExSgType_SourceBlendShape ) )
	{
		return BtFalse;
	}
	return BtTrue;
}

#endif // __ExSgNode_h__

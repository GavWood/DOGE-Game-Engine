////////////////////////////////////////////////////////////////////////////////
// ExScene.h

#pragma once
#include "BaArchive.h"
#include "BtTypes.h"
#include "TeTexture.h"
#include "PaPacker.h"
#include "SgNode.h"
#include "RsVertex.h"
#include "ExMaterialList.h"
#include "ExVertexBuffer.h"
#include "ExTextureList.h"
#include "ExIndexBuffer.h"
#include "ExCollisionMesh.h"
#include "ExCollisionAnalytical.h"
#include "ExSgNode.h"
#include <vector>

const BtU32 MaxVertexDeclarations = 1024;

class FCDocument;
class FCDSceneNode;
class FCDEntityInstance;
class FCDGeometry;
class FCDGeometryInstance;
class FCDController;
class FCDSkinController;
class FCDMorphController;
class MCCollada;

class ExScene : public ExResource
{

public:

	ExScene();

	void									Create();
	void									Export();
	virtual void							Destroy();
	void									MoveToVertexBuffers();

	// Accessors
	ExVertexBuffer*							pVertexBuffer( BtU32 vertexType );
	ExIndexBuffer*							pIndexBuffer();
	ExCollisionMesh*						pCollisionMesh();

	// Accessors
	BtU32									SceneResourceID();
	ExMaterialList&							Materials();
	BtU32									GetNumSceneNodes();
	const ExSgNode*							pSceneNode( BtU32 iIndex ) const;
	ExTextureList*							pTextureList();
	const ExCollisionAnalytical&			CollisionAnalytical() const;
	const ExCollisionMesh&					CollisionMesh() const;

	static BtBool							IsMetasequoia() { return m_isMetasequoia; }

	// HACK
	ExSgNode *FindRoot( ExSgNode *pNode );

protected:

	void									Update();
	void									GetAnimationLength();
	void									ExportResources();
	void									AddSceneNode( ExSgNode* pSceneNode );
	void									OptimiseGeometry();

	ExSgNode*								FindSceneNode( const BtChar* pSceneNode );
	ExSgNode*								FindSceneNode( const FCDSceneNode* pColladaNode );
	ExSgNode*								FindSceneNodeById( const BtChar* sceneNodeId );

	void									SetExportHierarchy();
	void									ChangeCoordinateSystem();
	void									SetCollisionMeshes();

	void									ExtractDocument();
	void									ExtractSceneNodes();
	void									ExtractHinges();
	void									ExtractPhysicsNodes();
	void									ExtractTransforms();
	void									ExtractLight( FCDEntityInstance* pEntity );
	void									ExtractHierarchy( FCDSceneNode* pSceneNode, ExSgNode* pCurrentNode );
	void									ExtractBones( const FCDSkinController* pSkinController );
	void									ExtractBlendShape( FCDEntityInstance* pEntityInstance, FCDController* pController );
	void									CopyBlendShapes();

	void									TranslateVertices( ExSgNode* pNode, MtMatrix4 m4Transform );

	void									MarkUpHierarchy();

	void									ExtractGeometry( FCDEntityInstance* pEntityInstance );
	void									ExtractSkin( const FCDEntityInstance* pEntityInstance );

	void									ExtractGeometry( FCDGeometryInstance* pGeometryInstance,
															 FCDGeometry* pGeometry );

	void									FlattenAnimation();
	void									ExtractToMaterialAndRenderGroups();

	void									SerialiseSceneNodes();
	void									ExportIndexBuffer();
	void									ExportVertexBuffer();
	void									ExportCollision();
	void									ExportAnimation();
	void									ExportPhysicsNodes();

private:

	FCDocument*								m_pDocument;

	// Rendering
	std::vector<BaRenderBlockFileData>		m_renderBlocks;
	std::vector<RsIndexedPrimitive>			m_primitives;

	// Animation
	std::vector<BaSgAnimationTimeFileData>		m_animationTimes;
	std::vector<BaSgAnimationTransformFileData>	m_animationTransforms;

	ExSgNode*								m_pCurrentSceneNode;
	std::vector<ExSgNode*>					m_pSgNodes;

	BtU32									m_nSceneResourceID;
	BtU32									m_nAnimationKeyResourceID;

	ExMaterialList							m_materialList;
	ExTextureList							m_textureList;

	BtFloat									m_maxAnimationDuration;
	BtFloat									m_animationFrameRate;

	ExVertexBuffer							m_vertexBuffer[MaxVertexDeclarations];
	ExIndexBuffer							m_indexBuffer;

	ExCollisionMesh							m_collisionMesh;
	ExCollisionAnalytical					m_collisionShapes;

	MCCollada*								m_pColladaWrapper;

	BtBool									m_isDynamicVB;
	BtBool									m_isBlender;
	static BtBool							m_isMetasequoia;
};

inline ExCollisionMesh* ExScene::pCollisionMesh()
{
	return &m_collisionMesh;
}

inline ExTextureList* ExScene::pTextureList()
{
	return &m_textureList;
}

inline BtU32 ExScene::SceneResourceID()
{
	return m_nSceneResourceID;
}

inline void ExScene::AddSceneNode( ExSgNode* pSceneNode )
{
	m_pSgNodes.push_back( pSceneNode );
}

inline ExMaterialList& ExScene::Materials()
{
	return m_materialList;
}

inline BtU32 ExScene::GetNumSceneNodes()
{
	return (BtU32) m_pSgNodes.size();
}

inline const ExSgNode* ExScene::pSceneNode( BtU32 iIndex ) const
{
	return m_pSgNodes[iIndex];
}

inline ExVertexBuffer*	ExScene::pVertexBuffer( BtU32 vertexType )
{
	return &m_vertexBuffer[vertexType];
}

inline ExIndexBuffer* ExScene::pIndexBuffer()
{
	return &m_indexBuffer;
}

inline const ExCollisionAnalytical& ExScene::CollisionAnalytical() const
{
	return m_collisionShapes;
}

inline const ExCollisionMesh& ExScene::CollisionMesh() const
{
	return m_collisionMesh;
}

////////////////////////////////////////////////////////////////////////////////
// HlModel.h

// Include guard
#pragma once

#include "BtTypes.h"
#include "MtSphere.h"

class SgNode;
class RsTexture;

class HlModel
{
public:

	static void						MeasureSphere( MtSphere &dimSphere, SgNode* pCurrentNode );
	static void						MeasureSphereRecursively( MtSphere &dimSphere, SgNode* pCurrentNode );
	static void						SetTextures( SgNode *pNode, BtU32 index, RsTexture *pTexture );
	static void						SetMaterialTechnique( SgNode *pNode, const BtChar *technique );
	static void						SetMaterialAlpha(SgNode *pNode, BtFloat alpha);
	static void						SetSortOrders(SgNode *pNode, BtU32 sortOrder);
	static void						RenderBones( SgNode *pSkin );
	static void						RenderChildBones( const MtMatrix4 &m4Transform, SgNode *pParent, SgNode *pChild );
};

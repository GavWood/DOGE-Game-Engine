////////////////////////////////////////////////////////////////////////////////
// HlModel.cpp

#include "HlModel.h"
#include "SgNode.h"
#include "SgMaterial.h"
#include "RsTexture.h"
#include "RsMaterial.h"
#include "SgMesh.h"
#include "SgSkin.h"
#include "HlDraw.h"
#include "BtBase.h"

static BtBool hasMeasured = BtFalse;

////////////////////////////////////////////////////////////////////////////////
// MeasureSphere

void HlModel::MeasureSphere( MtSphere &dimSphere, SgNode* pCurrentNode )
{
	hasMeasured = BtFalse;

	MeasureSphereRecursively( dimSphere, pCurrentNode );
}

////////////////////////////////////////////////////////////////////////////////
// MeasureSphere

void HlModel::MeasureSphereRecursively( MtSphere &dimSphere, SgNode* pCurrentNode )
{
	MtMatrix4 m4Translation;

	MtVector3 v3Translation = pCurrentNode->GetWorldTransform().GetTranslation();

	m4Translation.SetTranslation( v3Translation.x, v3Translation.y, v3Translation.z );

	if (pCurrentNode->pSkin() != BtNull)
	{
		MtSphere sphere = pCurrentNode->pSkin()->Sphere();

		BtAssert( sphere.GetRadius() );

		sphere.Transform(pCurrentNode->GetWorldTransform());
	
		if (!hasMeasured)
		{
			dimSphere = sphere;

			hasMeasured = BtTrue;
		}
		else
		{
			// Expand by the new sphere
			dimSphere.ExpandBy(sphere);
		}
	}

	if( pCurrentNode->GetMesh() != BtNull )
	{
		if( pCurrentNode->GetMesh()->GetNumMaterials() )
		{
			MtSphere sphere = pCurrentNode->GetMesh()->GetSphere();
			
			sphere.Transform( pCurrentNode->GetWorldTransform() );

			if( !hasMeasured )
			{
				dimSphere = sphere;

				hasMeasured = BtTrue;
			}
			else
			{
				// Expand by the new sphere
				dimSphere.ExpandBy( sphere );
			}
		}
	}

	// Cache the first child
	SgNode* pChild = pCurrentNode->pFirstChild();

	// Loop through the children
	while( pChild != BtNull )
	{
		MeasureSphere( dimSphere, pChild );

		// Move to the next child
		pChild = pChild->pNextSibling();
	}
}

////////////////////////////////////////////////////////////////
// SetTextures

void HlModel::SetTextures( SgNode *pNode, BtU32 index, RsTexture *pTexture )
{
	// Cache the materials
	SgMaterial* pMaterials = pNode->pMaterial();

	// Cache the number of materials
	BtU32 numMaterials = pMaterials->NumMaterials();

	for( BtU32 i=0; i<numMaterials; i++)
	{
		// Cache the material
		RsMaterial* pMaterial = pMaterials->GetMaterial( i );

		// Set the texture
		pMaterial->SetTexture( index, pTexture );
	}
	
	// Cache the first child
	SgNode* pChild = pNode->pFirstChild();

	// Loop through the children
	while( pChild != BtNull )
	{
		// Set the materials
		SetTextures( pChild, index, pTexture );

		// Move to the next child
		pChild = pChild->pNextSibling();
	}
}

////////////////////////////////////////////////////////////////
// SetSortOrders

void HlModel::SetSortOrders( SgNode *pNode, BtU32 sortOrder )
{
	if( pNode->HasMaterials() )
	{
		SgMaterial* pMaterials = pNode->pMaterial();

		// Cache the number of materials
		BtU32 numMaterials = pMaterials->NumMaterials();

		for( BtU32 i=0; i<numMaterials; i++)
		{
			// Cache the material
			RsMaterial* pMaterial = pMaterials->GetMaterial( i );

			// Set the sort order
			pMaterial->SetSortOrder( sortOrder );

			// Set the material
			pMaterials->SetMaterial( i, pMaterial );
		}
	}

	// Cache the first child
	SgNode* pChild = pNode->pFirstChild();

	// Loop through the children
	while( pChild != BtNull )
	{
		// Set the materials
		SetSortOrders( pChild, sortOrder );

		// Move to the next child
		pChild = pChild->pNextSibling();
	}
}

////////////////////////////////////////////////////////////////
// SetMaterialAlpha

void HlModel::SetMaterialAlpha(SgNode *pNode, BtFloat alpha )
{
	if(pNode->GetMesh())
	{
		// Cache the materials
		SgMaterial* pMaterials = pNode->pMaterial();

		// Cache the number of materials
		BtU32 numMaterials = pMaterials->NumMaterials();

		for(BtU32 i = 0; i < numMaterials; i++)
		{
			// Cache the material
			RsMaterial* pMaterial = pMaterials->GetMaterial(i)->GetDuplicate();

			RsColour colour = pMaterial->GetDiffuseColour();
			colour.Alpha( 0.5f );
			pMaterial->SetDiffuseColour( colour );
		}
	}

	// Cache the first child
	SgNode* pChild = pNode->pFirstChild();

	// Loop through the children
	while(pChild != BtNull)
	{
		// Set the materials
		SetMaterialAlpha(pChild, alpha);

		// Move to the next child
		pChild = pChild->pNextSibling();
	}
}

////////////////////////////////////////////////////////////////
// SetMaterialTechnique

void HlModel::SetMaterialTechnique( SgNode *pNode, const BtChar *technique )
{
	if( pNode->GetMesh() || pNode->pSkin() )
	{
		// Cache the materials
		SgMaterial* pMaterials = pNode->pMaterial();

		// Cache the number of materials
		BtU32 numMaterials = pMaterials->NumMaterials();

		for( BtU32 i=0; i<numMaterials; i++)
		{
			// Cache the material
			RsMaterial* pMaterial = pMaterials->GetMaterial( i )->GetDuplicate();

			if( strcmp( technique, "debug" ) == 0 )
			{
				// Cache the current technique name
				BtChar *currentTechniqueName = pMaterial->GetTechniqueName();
				(void)currentTechniqueName;
				int a=0;
				a++;
			}
			else
			{
				// Set the technique name
				pMaterial->SetTechniqueName( technique );

				// Set the material
				pMaterials->SetMaterial( i, pMaterial );
			}
		}
	}

	// Cache the first child
	SgNode* pChild = pNode->pFirstChild();

	// Loop through the children
	while( pChild != BtNull )
	{
		// Set the materials
		SetMaterialTechnique( pChild, technique );

		// Move to the next child
		pChild = pChild->pNextSibling();
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderChildBones

void HlModel::RenderChildBones( const MtMatrix4 &m4Transform, SgNode *pParent, SgNode *pChild )
{
	// Loop through the children
	while( pChild )
	{
		// Draw a line to the new child
		HlDraw::RenderLine( pParent->GetWorldTransform().GetTranslation() * m4Transform,
							 pChild->GetWorldTransform().GetTranslation() * m4Transform,
							 RsColour::WhiteColour() );

		RenderChildBones( m4Transform, pChild, pChild->pFirstChild() );

		// Move to the next child
		pChild = pChild->pNextSibling();
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderBones

void HlModel::RenderBones( SgNode *pSkin )
{
	SgNode *pSkeleton = pSkin->GetJoint(0);
    
	while( pSkeleton->pParent() )
	{
		pSkeleton = pSkeleton->pParent();
	}
	RenderChildBones( pSkin->GetWorldTransform(), pSkeleton, pSkeleton->pFirstChild() );
}
// Include guard
#pragma once

// Includes
#include "RsCamera.h"
#include "RsColour.h"
#include "BtLinkedList.h"
#include "RsPrimitive.h"

const BtU32 MaxSortOrders = 16;

class RsTexture;
class RsFont;
class RsSprite;
class RsShader;
class SgNode;
class SgMesh;
class SgSkin;
class RsMaterial;
class RsRenderTarget;
class RsTrueTypeFont;
class RsShaderParam;
struct RsVertex3;
struct BaMaterialBlockFileData;

struct RsFontRenderable : BtLinkedListItem<RsFontRenderable>
{
	RsShader							   *m_pShader;
	RsRenderTarget						   *m_pRenderTarget;
	RsVertex3							   *m_pVertex;
	RsFont								   *m_pFont;
	RsPrimitive							   *m_primitive;

	// TTF
	RsTrueTypeFont						   *m_pTrueTypeFont;
	RsTexture							   *m_pTexture;
	RsColour								m_colour;
	BtChar									m_text[256];
};

struct RsMaterialRenderable : BtLinkedListItem<RsMaterialRenderable>
{
	RsShader							   *m_pShader;
	RsRenderTarget						   *m_pRenderTarget;
	RsVertex3							   *m_pVertex;
	RsPrimitive							   *m_primitive;
	RsMaterial							   *m_pMaterial;

	RsColour								m_ambient;
	MtVector3								m_v3DirectionalLight;
	MtMatrix4								m_m4Projection;
	MtMatrix4								m_m4View;
	MtMatrix4								m_m4World;
};

struct RsTextureRenderable : BtLinkedListItem<RsTextureRenderable>
{
	RsShader							   *m_pShader;
	RsRenderTarget						   *m_pRenderTarget;
	RsVertex3							   *m_pVertex;
	RsTexture							   *m_pTexture;
	RsPrimitive							   *m_primitive;
};

struct RsSpriteRenderable : BtLinkedListItem<RsSpriteRenderable>
{	
	RsShader							   *m_pShader;
	RsRenderTarget						   *m_pRenderTarget;
	RsVertex3							   *m_pVertex;
	RsSprite							   *m_pSprite;
	RsPrimitive							   *m_primitive;
};

struct SgMeshRenderable : BtLinkedListItem<SgMeshRenderable>
{	
	RsShader							   *m_pShader;
	RsRenderTarget						   *m_pRenderTarget;
	SgMesh                                 *m_pMesh;
	BaMaterialBlockFileData*				m_pMaterialBlock;

	RsColour								m_ambient;
	MtVector3								m_v3DirectionalLight;
	MtMatrix4								m_m4ViewProjection;
	MtMatrix4								m_m4View;
	MtMatrix4								m_m4World;
};

struct SgSkinRenderable : BtLinkedListItem<SgSkinRenderable>
{	
	RsShader							   *m_pShader;
	RsRenderTarget						   *m_pRenderTarget;
	SgSkin                                 *m_pSkin;
	BaMaterialBlockFileData*				m_pMaterialBlock;

	RsColour								m_ambient;
	MtVector3								m_v3DirectionalLight;
	MtMatrix4								m_m4ViewProjection;
	MtMatrix4								m_m4View;
	MtMatrix4								m_m4World;
};

// Class Declaration
class RsRenderTarget
{
public:

		// Constructor
		RsRenderTarget();

		// Public functions
		void							Apply();

		static BtU32					GetNum();
		static RsRenderTarget		   *GetCurrent();

		// Accessors
		void							SetName( const BtChar *name );
		void							SetTexture( RsTexture *pTexture );
		RsTexture					   *GetTexture();

		void							SetCamera( const RsCamera &camera );
		const RsCamera				   &GetCamera();

		MtVector2						GetCameraDimension();

		BtBool							IsCleared() const;
		void							SetCleared( BtBool isCleared );

		BtBool							IsZCleared() const;
		void							SetZCleared( BtBool isZCleared );

		void							SetClearColour( const RsColour &colour );
		RsColour						GetClearColour() const;

protected:

		RsCamera						m_camera;
		BtBool							m_isCleared;
		BtBool							m_isZCleared;
		RsColour						m_clearColour;
		RsTexture					   *m_pTexture;
		BtChar							m_name[256];
		static RsRenderTarget		   *m_pCurrentRenderTarget;
};

////////////////////////////////////////////////////////////////////////////////
// RsManagerImpl.h

#pragma once
#include "BtArray.h"
#include "BtTypes.h"
#include "BtLinkedList.h"
#include "RsImpl.h"
#include "RsRenderTargetImpl.h"
#include "RsImpl.h"

// Class declaration
class RsImplWinGL : public RsImpl
{
public:

	void											   *GetHandle() { return BtNull; }

	// Public functions
	void 												Create();
	void 												Destroy();
	void												ResetRenderables();

	void												EmptyRenderTargets();
	void												BeginScene();
	RsRenderTarget									   *GetNewRenderTarget();

	void 												Render();
	void												SetDimension( const MtVector2 &v2Dimension );

	// Renderables
	RsFontRenderable								   *AddFont();
	RsTextureRenderable								   *AddTexture();
	RsMaterialRenderable							   *AddMaterial();
	RsSpriteRenderable								   *AddSprite();
	SgMeshRenderable								   *AddMesh();
	RsPrimitive                                        *AddPrimitive();
    SgSkinRenderable                                   *AddSkin();

	// Vertex
	RsVertex3										   *StartVertex();
	void											    EndVertex( BtU32 vertexCount );

	// Accessors
	RsCaps											   *GetCaps();
    static BtU32                                        GetVertexBuffer();
//	RsVertexBufferWin32								   *GetVertexBuffer();
//	RsIndexBufferWin32								   *GetIndexBuffer();

private:

	// Private functions
	void												CreateOnDevice();
	void												RemoveFromDevice();

	// Private members
	BtArray<RsRenderTargetWinGL, MaxRenderTargets>		m_renderTargets;
	BtArray< RsPrimitive, 4096>                         m_primitives;
    static BtU32                                        m_vertexBuffer;
};

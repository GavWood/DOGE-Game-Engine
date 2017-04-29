////////////////////////////////////////////////////////////////////////////////
// RsImplWin32GL.h

#ifndef __RsImplWin32GL_h__
#define __RsImplWin32GL_h__

#include "BtTypes.h"
#include "BtLinkedList.h"
#include "RsImpl.h"
#include "RsRenderTargetWinGL.h"

const BtU32 MaxRenderTargets = 1024;

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

	void												RenderLine( const MtVector2 &v2First, const MtVector2 &v2Second, const RsColour &colour, BtU32 sortOrder );
	void												RenderTriangleFan( const MtVector2 *pVertex, BtU32 numVertex, const RsColour &colour, BtU32 sortOrder ) {};

	void												SetDimension( const MtVector2 &v2Dimension );

	// Renderables
	RsFontRenderable								   *AddFont();
	RsTextureRenderable								   *AddTexture();
	RsMaterialRenderable							   *AddMaterial();
	RsSpriteRenderable								   *AddSprite();
	SgMeshRenderable								   *AddMesh();
	RsPrimitiveWinGL								   *AddPrimitive();
	SgSkinRenderable								   *AddSkin();

	// Vertex
	RsVertex3										   *StartVertex();
	void											    EndVertex( BtU32 vertexCount );

	// Accessors
	RsCaps											   *GetCaps();
	static BtU32									   GetVertexBuffer();
										
private:

	// Private functions
	void												CreateOnDevice();
	void												RemoveFromDevice();

	// Private members
	BtArray<RsRenderTargetWinGL, MaxRenderTargets>		m_renderTargets;
	BtArray< RsPrimitiveWinGL, 4096>					m_primitives;
	static BtU32										m_vertexBuffer;
};

#endif // __RsImplWin32GL_h__

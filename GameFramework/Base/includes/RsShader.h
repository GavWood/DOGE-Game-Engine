////////////////////////////////////////////////////////////////////////////////
/// RsShader.h

#pragma once
#include "BtTypes.h"
#include "RsTexture.h"
#include "RsMaterial.h"
#include "RsPrimitive.h"
#include "MtMatrix4.h"

enum RsHandles
{
	RsHandles_World,
	RsHandles_View,
	RsHandles_ViewInverse,
	RsHandles_WorldView,
	RsHandles_ViewScreen,
	RsHandles_WorldViewScreen,
	RsHandles_CameraPosition,
	RsHandles_BlendWeights,
	RsHandles_WorldViewInverseTranspose,
	RsHandles_ViewInverseTranspose,
	RsHandles_Light0Direction,
	RsHandles_LightAmbient,
	RsHandles_TextureMatrix0,
	RsHandles_BoneMatrix,
	RsHandles_Colour,
	RsHandles_MAX,
};

// Class Declaration
class RsShader : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Shader );

	void						Apply();
	static RsShader			   *GetCurrent();

	virtual void 				SetTechnique( const BtChar *pTechniqueName ) = 0;
	virtual void				SetMaterial( RsMaterial *pMaterial ) = 0;
	virtual RsShader*			pDuplicate() = 0;

	virtual void				SetParams(BtU8 *paramData, BtU32 length) = 0;

	virtual void				SetAmbient( const RsColour &colour ) = 0;
	virtual void				SetDirectionalLight( const MtVector3 &v3Direction ) = 0;

	// Deprecated
	virtual void 				SetTexture(const BtChar *pName, const RsTexture *pTexture) = 0;
	virtual void 				SetTexture(const BtChar *pName) = 0;
	virtual void 				SetMatrix(const BtChar *pName, const MtMatrix4 &m4Matrix) = 0;
	virtual void				SetMatrix(const BtChar *pMatrixName, const MtMatrix4 *pMatrix, BtU32 nCount) = 0;
	virtual void 				SetMatrix(RsHandles Handle, const MtMatrix4 &m4Matrix) = 0;
	virtual void				SetFloats(const BtChar *pArrayName, const BtFloat *pArray, BtU32 nCount) = 0;
	virtual void				SetFloats(RsHandles Handle, const BtFloat *pArray, BtU32 nCount) = 0;
	virtual void				SetFloat(const BtChar *pName, BtFloat value) = 0;
	virtual void				SetInteger(const BtChar *pName, BtU32 value) = 0;
	virtual void				SetColour(const BtChar *pColourName, const RsColour& colour) = 0;
	virtual void				Draw(const RsPrimitive *pPrimitive) = 0;
	virtual void				Draw(const RsIndexedPrimitive *pPrimitive) = 0;

private:

	static RsShader			   *m_pCurrentShader;
};

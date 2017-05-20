////////////////////////////////////////////////////////////////////////////////
// RsShaderWinGL.h

#pragma once
#include "BtArray.h"
#include "BtTypes.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"
#include "MtVector4.h"
#include "RsTextureWinGL.h"
#include "RsShader.h"
#include "RsColour.h"
#include "RsPrimitiveWinGL.h"

struct BaResourceHeader;

class RsCamera;

const BtU32 RsShaderPassThrough = 0;
const BtU32 RsShaderLit = 1;
const BtU32 RsShaderTextured = 2;
const BtU32 RsShaderLitTextured = 3;
const BtU32 RsShaderSkin = 4;
const BtU32 RsProjectedTexture = 5;
const BtU32 RsShadow = 6;

// Class Declaration
class RsShaderWinGL : public RsShader
{
public:

	//virtual
	void								Apply();

	RsShader*							pDuplicate();
	void								FixPointers(BtU8 *pFileData, BaArchive *pArchive);

	void								SetAmbient( const RsColour &colour );
	void								SetDirectionalLight( const MtVector3 &v3Direction );
	RsColour							GetAmbient();
	MtVector3							GetDirectionalLight();

	void								SetParams(BtU8 *paramData, BtU32 length) {};

	void								CreateOnDevice();
	void								RemoveFromDevice();
	void								Remove();

	// Setting values on shader

	void								SetTechnique( const BtChar* pTechniqueName );
	void								SetMaterial( RsMaterial* pMaterial );
	void								SetTexture( const BtChar* pTextureName, const RsTexture* pTexture );
	void								SetTexture( const BtChar* pTextureName );
	void								SetMatrix( const BtChar* pMatrix, const MtMatrix4& m4Matrix );
	void								SetMatrix( const BtChar* pMatrixName, const MtMatrix4* pMatrix, BtU32 nCount );
	void								SetColour( const BtChar* pColourName, const RsColour& colour );
	void								SetFloats( const BtChar* pArrayName, const BtFloat* pArray, BtU32 nCount );
	void								SetFloat( const BtChar* pName, BtFloat value );
	void								SetInteger( const BtChar* pName, BtU32 value );
	void								SetCamera( const RsCamera &camera );
	void								SetLights(BtU32 index);

	// By handle
	void								SetMatrix( RsHandles Handle, const MtMatrix4& m4Matrix );
	void								SetFloats( RsHandles Handle, const BtFloat* pArray, BtU32 nCount );
	
	void								Draw( const RsPrimitive* pPrimitive );
	void								Draw( const RsIndexedPrimitive* pPrimitive );

private:

	void								Commit();

	BaShaderFileData*					m_pFileData;

	BtS32								m_program[8];
	BtS32								m_sampler[8];

	BtS32								m_handles[8][RsHandles_MAX];
	MtVector4							m_floats[RsHandles_MAX];
};

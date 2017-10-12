////////////////////////////////////////////////////////////////////////////////
// BaFileData.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "RsColour.h"
#include "MtMatrix4.h"
#include "MtVector3.h"
#include "MtPlane.h"
#include "MtAABB.h"
#include "MtSphere.h"

class BaArchive;
class BaResource;
class RsTexture;
class RsMaterial;
class DyCollisionMesh;
class DyCollisionAnalytical;
class SgNode;

const BtU32 MaxBoneMatrices = 10;
const BtU32 MaxBlendShapes = 10;
const BtU32 MaxFilePath = 1024;
const BtU32 MaxFlashTextures = 32;
const BtU32 MaxFontTexturePages = 32;
const BtU32 MaxFontCharacters = 256;
const BtU32 MaxSceneNodeName = 64;
const BtU32 MaxShaders = 32;
const BtU32 MaxSpritesInResource = 64;
const BtU32 MaxTextures = 8;

enum RsRenderFlags
{
	RsRF_Hidden			 = 1 << 1,
	RsRF_Visible		 = 1 << 2,
	RsRF_ForceVisible	 = 1 << 3,
	RsRF_Max			 = 1 << 4,
};

enum RsTextureFlags
{
	RsTF_Mipmap			  = 1 << 0,
	RsTF_RenderTarget	  = 1 << 1,
	RsTF_FloatingPoint16  = 1 << 2,
	RsTF_FloatingPoint32  = 1 << 3,
	RsTF_FloatingPoint1   = 1 << 4,
	RsTF_FloatingPoint2   = 1 << 5,
	RsTF_FloatingPoint4   = 1 << 6,
	RsTF_Red			  = 1 << 7,
	RsTF_DepthBuffer	  = 1 << 8,
	RsTF_CubeMap		  = 1 << 9,
	RsTF_Editable		  = 1 << 10,
	RsTF_BackBufferSize	  = 1 << 11,
	RsTF_Square			  = 1 << 12,
	RsTF_SystemMemory	  = 1 << 13,
	RsTF_Alpha			  = 1 << 14,
	RsTF_Indexed		  = 1 << 15,
	RsTF_IndexedAlpha	  = 1 << 16,
	RsTF_PVR4			  = 1 << 17,
	RsTF_NearestNeighbour = 1 << 18,
	RsTF_DXT			  = 1 << 19,
	RsTF_ClampU			  = 1 << 20,
	RsTF_ClampV			  = 1 << 21,
	RsTF_WrapU			  = 1 << 22,
	RsTF_WrapV			  = 1 << 23,
	RsTF_Max			  = 1 << 24
};

enum RsMaterialFlags
{
	RsMaterial_ZTest		  = 1 << 0,
	RsMaterial_ZWrite		  = 1 << 1,
	RsMaterial_Lit			  = 1 << 2,
	RsMaterial_Mask			  = 1 << 3,
	RsMaterial_Mipmap		  = 1 << 4,
	RsMaterial_Transparent	  = 1 << 5,
	RsMaterial_DoubleSided	  = 1 << 6,
	RsMaterial_Fog			  = 1 << 7,
	RsMaterial_BlendShape	  = 1 << 8,
	RsMaterial_Sprite		  = 1 << 9,
	RsMaterial_NormalMap	  = 1 << 10,
	RsMaterial_EnvironmentMap = 1 << 11,
	RsMaterial_SpecularMap    = 1 << 12,
	RsMaterial_Max			  = 1 << 13
};

enum RsShaderFlags
{
	RsST_D3DEffect = 1 << 0,
	RsST_CGFX	   = 1 << 1,
};

enum VertexBufferFlags
{
	RsVB_Dynamic = 1 << 0,
	RsVB_MAX   = 1 << 1,
};

enum VertexType
{
	VT_Position = 1 << 0,
	VT_Normal = 1 << 1,
	VT_Colour = 1 << 2,
	VT_UV0 = 1 << 3,
	VT_UV1 = 1 << 4,
	VT_Skin = 1 << 5,
	VT_BlendShape = 1 << 6,
	VT_Tangent = 1 << 7,
	VT_Binormal = 1 << 8,
	VT_MAX
};

#pragma pack(push,1)
#pragma pack(1)

////////////////////////////////////////////////////////////////////////////////
// Test structure for data organisation

struct BaTest
{
    BtU8                            m_byte;         // 1
    BtU16                           m_short;        // 2
    BtU32                           m_word;         // 4
    BtU64                           m_doubleWord;   // 8
};

////////////////////////////////////////////////////////////////////////////////
// String structures

struct BaStringFileData
{
	BtU32							nNumStrings;
};

////////////////////////////////////////////////////////////////////////////////
// Shader structure

struct BaShader
{
	BtChar							m_name[32];
	BtU32							m_offset;
	BtU32							m_size;
};

struct BaShaderFileData
{
	BtU32							m_count;
	BaShader						m_shaders[MaxShaders];
};

////////////////////////////////////////////////////////////////////////////////
// Texture structure

struct LBaTextureFileData
{
	BtU32							m_flags;
	BtU32							m_nMipMaps;
};

struct LBaMipMapFileData
{
	BtU32							m_nTextureSize;
	BtU32							m_nPitch;
	BtU32							m_nBytesPerPixel;
	BtU32							m_nWidth;
	BtU32							m_nHeight;
	BtU32							m_originalWidth;
	BtU32							m_originalHeight;
	BtFloat							m_aspectRatio;

	union
	{
	BtU8*							m_pTextureMemory;
	BtU64							m_pointerPad;
	};
};

////////////////////////////////////////////////////////////////////////////////
// BaVertexBufferFileData

struct BaVertexBufferFileData
{
	BtU32							m_type;
	BtU32							m_flags;
	BtU32							m_nVertices;
	BtU32							m_nStride;
	BtChar							m_description[256];
};

////////////////////////////////////////////////////////////////////////////////
// BaIndexBufferFileData

struct BaIndexBufferFileData
{
	BtU32							m_type;
	BtU32							m_nIndices;
	BtChar							m_description[256];
};

////////////////////////////////////////////////////////////////////////////////
// Sprite structures

struct BaSprite
{
	BtFloat							m_fX;
	BtFloat							m_fY;
	BtFloat							m_width;
	BtFloat							m_height;
	BtU32							m_nTop;
	BtU32							m_nLeft;
	BtU32							m_nBottom;
	BtU32							m_nRight;
};

struct LBaSpriteFileData
{
	BtU32							m_nNumSprites;
	BtU32							m_nNumMasks;
	
	BtU32							m_nMaxWidth;
	BtU32							m_nMaxHeight;

	BaSprite						m_sprites[MaxSpritesInResource];

	union 
	{
		RsTexture*					m_pTexture;
		BtU64						m_pad1;
	};

	BtU32							m_nTexture;
	BtU32							m_pad2;
};

////////////////////////////////////////////////////////////////////////////////
// Font structures

struct LBaFontChar
{
	BtFloat							m_U0;
	BtFloat							m_V0;
	BtFloat							m_U1;
	BtFloat							m_V1;
	BtFloat							m_fWidth;
	BtFloat							m_fHeight;
	BtU32							m_nXOffset;
	BtU32							m_nYOffset;
	BtU32							m_nXAdvance;
	BtU32							m_nPage;
};

struct LBaFontFileData
{
	BtU32							m_isTTFFont;
	BtU32							m_pad1;

	// TTF renderer that can use an external library

	// Bespoke font renderer that just uses sprite positions
	BtU32							m_nTextHeight;
	BtU32							m_nTexturePages;
	LBaFontChar						m_characters[MaxFontCharacters];

	union
	{
	RsTexture*						m_pTextures[MaxFontTexturePages];
	BtU64							m_pad[MaxFontTexturePages];
	};

	BtU32							m_nTextures[MaxFontTexturePages];
};

////////////////////////////////////////////////////////////////////////////////
// Sound structure

struct BaSound
{
	BtU32							m_wFormatTag;        /* format type */
	BtU32							m_nChannels;         /* number of channels (i.e. mono, stereo...) */
	BtU32							m_nSamplesPerSec;    /* sample rate */
	BtU32							m_nAvgBytesPerSec;   /* for buffer estimation */
	BtU32							m_nBlockAlign;       /* block size of data */	
	BtU32							m_nBitsPerSample;	// Bits per sample
	BtU32							m_nSize;			// Size of sample
	BtU32							m_pad;
};

////////////////////////////////////////////////////////////////////////////////
// BaMaterialFileData

struct BaMaterialFileData
{
	BtChar							m_name[64];
	BtChar							m_techniqueName[64];

	BtU32							m_nFileDataSize;

	RsColour						m_diffuseColour;

	union
	{
		RsTexture*					m_pTexture[MaxTextures];
		BtU64						m_pad[MaxTextures];
	};
	BtU32							m_nTexture[MaxTextures];

	BtU32							m_nPasses;
	BtU32							m_flags;					// RsMaterial
	BtU32							m_nSortOrder;
	BtU32							m_vertexType;				// Vertex type. Will be replaced with FX/CG Shader

	// Parameters
	MtMatrix4						m_m4Transform;
};

////////////////////////////////////////////////////////////////////////////////
// BaRenderBlockFileData

struct BaRenderBlockFileData
{
	BtS32							m_bonePalette[MaxBoneMatrices];
	BtU32							m_bonePaletteSize;
	BtU32							m_nStartPrimitiveBlock;
	BtU32							m_nEndPrimitiveBlock;
	BtU32							m_pad;
};

////////////////////////////////////////////////////////////////////////////////
// BaMaterialsFileData

struct BaMaterialsFileData
{
	BtU32							m_nMaterials;
	BtU32							m_pad;
};

////////////////////////////////////////////////////////////////////////////////
// BaMaterialBlockFileData

struct BaMaterialBlockFileData
{
	union
	{
		RsMaterial                 *m_pMaterial;
		BtU64						m_pointerPad;
	};
	BtU32							m_nMaterial;
	BtU32							m_pad;

	BtU32							m_nStartRenderBlock;
	BtU32							m_nEndRenderBlock;
};

////////////////////////////////////////////////////////////////////////////////
// BaSceneFileData

struct BaSceneFileData
{
	BtU32							m_nRenderBlocks;
	BtU32							m_nPrimitives;

	union
	{
		BaResource				   *m_pVertexBuffer[1024];	// MaxVertexDeclarations
		BtU64						m_pointerPad1[1024];
	};
	BtU32							m_nVertexBuffer[1024];

	union
	{
		BaResource			       *m_pIndexBuffer;
		BtU64						m_pointerPad2;
	};
	BtU32							m_nIndexBuffer;

	union
	{
		DyCollisionMesh            *m_pCollisionMesh;
		BtU64						m_pointerPad3;
	};
	BtU32							m_nCollisionMesh;

	union
	{
		DyCollisionAnalytical      *m_pCollisionAnalytical;
		BtU64						m_pointerPad4;
	};
	BtU32							m_nCollisionAnalytical;
};

struct BaSgNodeFileData
{
	BtU32							m_nodeType;
	BtChar							m_name[MaxSceneNodeName];

	BtU32							m_nChildren;
	BtU32							m_nFileDataSize;

	union
	{
	SgNode*							m_pParent;
	BtU64							m_pointerPad1;
	};
	BtU32							m_nParent;

	union
	{
	SgNode*							m_pFirstChild;
	BtU64							m_pointerPad2;
	};
	BtU32							m_nFirstChild;

	union
	{
	SgNode*							m_pNextSibling;
	BtU64							m_pointerPad3;
	};
	BtU32							m_nNextSibling;

	union
	{
	BaResource*						m_pScene;
	BtU64							m_pointerPad4;
	};

	BtU32							m_nScene;
	BtU32							m_renderFlags;

	MtMatrix4						m_m4Local;
	MtMatrix4						m_m4World;
};

struct BaSgAnimationTimeFileData
{
	BtFloat							m_time;
	BtU32							m_iTransformStart;
	BtU32							m_iTransformEnd;
	BtU32							m_pad;
};

struct BaSgAnimationTransformFileData
{
	BtS32							m_iBone;
	union
	{
		SgNode*						m_pNode;
		BtU64						m_nodeIndex;
	};
	MtMatrix4						m_m4Transform;
};

struct BaSgAnimationFileData
{
	BtU32							m_duration;
	BtU32							m_maxFrames;
	BtFloat							m_frameRate;
	BtU32							m_pad;
};

struct BaJointLookup
{
	union
	{
	SgNode*							m_pJoint;
	BtU64							m_pointerPad;
	};

	BtU32							m_nJoint;
	BtU32							m_pad;
};

struct BaSgSkinFileData
{
	MtAABB							m_AABB;
	MtSphere						m_sphere;
	BtBool							m_bVisible;

	BaJointLookup					m_skeleton[64];
	BtU32							m_numJoints;

	BtU32							m_nMaterials;
};

struct BaSgBoneFileData
{
	MtMatrix4						m_m4InverseBindPose;
};

struct BaSgLightFileData
{
	BtFloat							m_intensity;
	BtU32							m_type;
	RsColour						m_colour;
	BtU32							m_isLit;
	BtFloat							m_fallOffAngle;
	BtFloat							m_outerAngle;
};

struct BaSgCameraFileData
{
	BtFloat							m_fNearPlane;
	BtFloat							m_fFarPlane;
	BtFloat							m_fFieldOfView;
	BtU32							m_pad;
};

struct BaSgBlendShapeFileData
{
	BtU32							m_items;
	BtFloat							m_weight[MaxBlendShapes];
};

struct BaSgMeshFileData
{
	MtAABB							m_AABB;
	MtSphere						m_sphere;
	BtU32							m_nMaterials;
	BtU32							m_hasEnvmap;
};

struct BaSgRigidBodyFileData
{
	BtFloat							m_mass;
	MtVector3						m_v3CenterOfMass;
	BtU32							m_isDynamic;
	BtFloat							m_friction;
	BtFloat							m_restitution;
};

struct BaSgCollisionFileData
{
	BtU32							m_pBodies;
	BtU32							m_startShape;
	BtU32							m_endShape;
	BtU32							m_shapes;
};

struct BaCollisionAnalytical
{
	BtU32							m_numShapes;
	BtU32							m_pad;
};

struct BaFlashData
{
	BtU32							m_numTextures;

	union
	{
	RsTexture*						m_pTextures[MaxFlashTextures];
	BtU64							m_pointerPad[MaxFlashTextures];
	};

	BtU32							m_nTextures[MaxFlashTextures];
};

#pragma pack(pop) 

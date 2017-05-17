////////////////////////////////////////////////////////////////////////////////
// BaArchive

// Includes
#include <new>
#include "BtBase.h"
#include "BtString.h"
#include "BtPrint.h"
#include "BtTypes.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "BaUserData.h"
#include "BaResource.h"
#include "BtMemory.h"
#include "BtCompressedFile.h"
#include "ApConfig.h"

#include "DyCollisionMesh.h"
#include "DyCollisionAnalytical.h"

// WinGL
#include "SdSoundWinGL.h"
#include "RsMaterialWinGL.h"
#include "RsSpriteWinGL.h"
#include "RsTextureWinGL.h"
#include "RsFontWinGL.h"
#include "RsShaderWinGL.h"
#include "RsSceneWinGL.h"
#include "SgNodeWinGL.h"
#include "SgAnimatorWinGL.h"
#include "RsVertexBufferWinGL.h"
#include "RsIndexBufferWinGL.h"
#include "RsFlashWinGL.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// CreateResourceInstance

void BaArchive::CreateResourceInstance( BaResourceType resourceType, BtU8 *pMemory )
{
	// Cache the platform
	ApPlatform platform = ApConfig::GetPlatform();

	// Validate the platform
	BtAssert( platform != ApPlatform_Max );

	switch( resourceType )
	{
	case BaRT_CollisionMesh:
		{
			new(pMemory) DyCollisionMesh;
		}
		break;

	case BaRT_CollisionAnalytical:
		{
			new(pMemory) DyCollisionAnalytical;
		}
		break;

	case BaRT_VertexBuffer:
		new (pMemory) RsVertexBufferWinGL;
		break;

	case BaRT_IndexBuffer:
		new (pMemory) RsIndexBufferWinGL;
		break;

	case BaRT_Material:
		new( pMemory ) RsMaterialWinGL;
		break;

	case BaRT_UserData:
		new( pMemory ) BaUserData;
		break;

	case BaRT_Sprite:
		new( pMemory ) RsSpriteWinGL;
		break;

	case BaRT_Font:
		new( pMemory ) RsFontWin32GL;
		break;

	case BaRT_Texture:
		new( pMemory ) RsTextureWinGL;
		break;

	case BaRT_Sound:
		new( pMemory ) SdSoundWinGL;
		break;

	case BaRT_Scene:
		new (pMemory) RsSceneWinGL;
		break;

	case BaRT_SgNode:
		new( pMemory ) SgNodeWinGL;
		break;

	case BaRT_Animator:
		new( pMemory ) SgAnimatorWinGL;
		break;

	case BaRT_Shader:
		new( pMemory ) RsShaderWinGL;
		break;

	case BaRT_Flash:
		new( pMemory ) RsFlashWinGL;
		break;

	default:
		new( pMemory ) BaResource;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// BaArchive.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BtLinkedList.h"
#include "BaDuplicate.h"

const BtU32 LMaxArchiveName  = 72;
const BtU32 LMaxResourceName = 68;

//////////////////////////////////////////////////////////////////////////
// BaResourceType

enum BaResourceType
{
	BaRT_NotSet = 0,
	BaRT_Strings = 1,
	BaRT_Texture = 2,
	BaRT_Sprite = 3,
	BaRT_Mask = 4,
	BaRT_Font = 5,
	BaRT_Shader = 6,
	BaRT_Input = 7,
	BaRT_UserData = 8,
	BaRT_Material = 9,
	BaRT_VertexBuffer = 10,
	BaRT_IndexBuffer = 11,
	BaRT_RenderBlock = 12,
	BaRT_Sound = 13,
	BaRT_Scene = 14,
	BaRT_SgNode = 15,
	BaRT_SgMesh = 16,
	BaRT_SgRigidBody = 17,
	BaRT_SgCollision = 18,
	BaRT_Animator = 19,
	BaRT_AnimationKeys = 20,
	BaRT_CollisionMesh = 21,
	BaRT_CollisionAnalytical = 22,
	BaRT_Flash = 23,
	BaRT_Max = 24,
};

struct BaArchiveHeader
{
	BtChar						m_szTitle[LMaxArchiveName];
	BtU32						m_nNumResources;
	BtU32						m_nDataAndInstanceSize;
};

struct BaResourceHeader
{
	BtChar						m_szTitle[LMaxResourceName];
	BaResourceType				m_type;
	
	BtU32						m_nInstanceOffset;
	BtU32						m_nInstanceAlignment;

	BtU32						m_nInstanceSize;
	BtU32						m_nResourceOffset;

	BtU32						m_nResourceAlignment;
	BtU32						m_nResourceSize;

	BtU32						m_nResourceID;
	BtU32						m_resourceFlags;

	BtU32						m_align1;
	BtU32						m_align2;
};

class RsFont;
class RsTexture;
class RsSprite;
class RsShader;
class RsMaterial;
class RsFlash;
class BaUserData;
class SdSound;
class BaResource;
class SgNode;
class SgCollision;
class SgRigidBody;
class SgAnimator;
class BaArchive;

struct BaArchiveNode : public BtLinkedListItem<BaArchiveNode>
{
	BaArchiveNode()
	{
		m_pArchive = BtNull;
	}

	BaArchiveNode(BaArchive* pArchive)
	{
		m_pArchive = pArchive;
	}

	BaArchive* m_pArchive;
};

// Class definition
class BaArchive
{
public:

	// Constructor
	BaArchive();

	// Public functions
	void						Load( const BtChar* archiveName );
	void						Unload();
	static void					CheckResources();

	// Accessors
	BtU8					   *GetResource( BaResourceType eType, const BtChar* szTitle ) const;
	BtU8					   *GetResource( BtU32 nResourceID ) const;
	BtChar					   *GetFilename() { return m_filename; }


	RsFont					   *GetFont( const BtChar* name );
	RsTexture				   *GetTexture( const BtChar* name );
	RsSprite				   *GetSprite( const BtChar* name );
	RsShader				   *GetShader( const BtChar *name );
	RsMaterial				   *GetMaterial( const BtChar *name );
	SgNode					   *GetNode( const BtChar *name );
	BaUserData				   *GetUserData( const BtChar* name );
	SdSound					   *GetSound( const BtChar* name );
	RsFlash					   *GetFlash( const BtChar* name );
	SgAnimator				   *GetAnimator( const BtChar *name );

	void						AddDuplicate( BaResource* pResource );
	BtBool						IsLoaded();

	// Debug
	static BtLinkedList<BaArchiveNode>	GetArchives() { return m_archives; }

private:

	void						LoadFile( const BtChar* archiveName );
	void						CreateResources();
	void						CreateResourceInstance( BaResourceType resourceType, BtU8 *pMemory );
	void						FixPointers();
	void						ValidateResources();

	BaArchiveNode				m_archiveNode;
	static BtLinkedList<BaArchiveNode>	m_archives;

	BtU8*						m_pMemory;
	BtLinkedList<BaDuplicate>	m_duplicates;
	BtChar						m_filename[256];
	BtBool						m_isLoaded;
};

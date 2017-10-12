////////////////////////////////////////////////////////////////////////////////
/// DyCollisionAnalytical.h

#pragma once
#include "BaFileData.h"
#include "BaResource.h"
#include "BtTypes.h"

struct SgCollisionShape;

// Class Declaration
class DyCollisionAnalytical : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_CollisionAnalytical );

	void						FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	const SgCollisionShape*		pCollisionShape( BtU32 iShape ) const;
	BtU32						NumShapes() const;

private:

	friend class BaArchiveManager;

    union
    {
	BaCollisionAnalytical*		m_pFileData;
    BtU64                       m_pad1;
    };
	
    BtU32						m_numShapes;
   	SgCollisionShape**			m_pShapes;
};

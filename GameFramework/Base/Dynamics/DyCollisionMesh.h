////////////////////////////////////////////////////////////////////////////////
/// DyCollisionMesh.h

#pragma once
#include "BtTypes.h"
#include "BaResource.h"
#include "SgCollision.h"

// Class Declaration
class DyCollisionMesh : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_CollisionMesh );

	void							FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	BtFloat*						pVertex() const;
	BtU32*							pIndex() const;

private:

	friend class BaArchiveManager;

	BaSgCollisionMesh*				m_pFileData;
	BtFloat*						m_pVertex;
	BtU32*							m_pIndex;
};

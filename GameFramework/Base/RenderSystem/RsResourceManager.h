////////////////////////////////////////////////////////////////////////////////
/// RsResourceManagerDX11.h

#pragma once
#include "BtTypes.h"
#include "BtLinkedList.h"

class BaResource;

////////////////////////////////////////////////////////////////////////////////
// BaResourceItem

struct BaResourceItem : public BtLinkedListItem< BaResourceItem >
{
	BaResource* m_pResource;
};

// Class Declaration
class RsResourceManager
{
public:

	RsResourceManager();

	void									CreateResources();
	void									RemoveResources();
	void									AddResource( BaResource* pResource );
	void									RemoveResource( BaResource* pResource );

private:

	BtLinkedList<BaResourceItem>			ResourceList_;
};

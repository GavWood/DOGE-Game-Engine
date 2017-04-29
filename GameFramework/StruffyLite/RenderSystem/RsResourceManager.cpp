////////////////////////////////////////////////////////////////////////////////
// RsResourceManager.cpp

// Includes
#include "BaResource.h"
#include "BaArchive.h"
#include "RsResourceManager.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

RsResourceManager::RsResourceManager()
{
}

///////////////////////////////////////////////////////////////////////////////
// CreateResources

void RsResourceManager::CreateResources()
{
	BaResourceItem* pResourceItem = ResourceList_.pHead();

	while( pResourceItem != BtNull )
	{
		pResourceItem->m_pResource->CreateOnDevice();

		pResourceItem = pResourceItem->pNext();
	}
}

//////////////////////////////////////////////////////////////////////////////
// RemoveResources

void RsResourceManager::RemoveResources()
{
	BaResourceItem* pResourceItem = ResourceList_.pHead();

	while( pResourceItem != NULL )
	{
		pResourceItem->m_pResource->RemoveFromDevice();

		pResourceItem = pResourceItem->pNext();
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddResource

void RsResourceManager::AddResource( BaResource* pResource )
{
	BaResourceItem* pResourceItem = new BaResourceItem;
	pResourceItem->m_pResource = pResource;
	ResourceList_.AddTail( pResourceItem );
}

////////////////////////////////////////////////////////////////////////////////
// RemoveResource

void RsResourceManager::RemoveResource( BaResource* pResource )
{
	BaResourceItem* pResourceItem = ResourceList_.pHead();

	while( pResourceItem != NULL )
	{
		if( pResourceItem->m_pResource == pResource )
		{
			ResourceList_.Remove( pResourceItem, BtTrue );
			break;
		}
		pResourceItem = pResourceItem->pNext();
	}
}

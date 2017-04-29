////////////////////////////////////////////////////////////////////////////////
// BaDuplicate.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BtLinkedList.h"

class BaResource;

struct BaDuplicate : public BtLinkedListItem<BaDuplicate>
{
	BaDuplicate( BaResource* pResource )
	{
		m_pResource = pResource;
	}

	BaResource*						m_pResource;
};

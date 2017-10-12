////////////////////////////////////////////////////////////////////////////////
// BtQueue.h

#pragma once
#include "BtBase.h"
#include "BtTypes.h"

template< class T, int MaxQueueSize> class BtQueue
{
public:

	BtQueue();

	void							Empty();
	BtBool							IsRoom() const;
	T*								Push( const T& Data );
	T*								PushFront(const T& Data);
	T								Pop();
	T&								Peek();
	T&								Peek( BtU32 iIndex );
	BtU32							GetItemCount() const;
	BtU32							GetMaxItems() const;

private:

	struct BtQueueItem
	{
		BtQueueItem*				m_pNext;
		T							m_item;
	};

	BtQueueItem*					m_pHead;
	BtQueueItem*					m_pBack;
	BtU32							m_nItems;

    BtQueueItem						m_queue[MaxQueueSize];
	BtU32							m_nCurrentItem;
};

//////////////////////////////////////////////////////////////////////////
// Constructor

template< class T, int MaxQueueSize > BtQueue< T, MaxQueueSize >::BtQueue()
{
	Empty();
}

//////////////////////////////////////////////////////////////////////////
// Empty

template< class T, int MaxQueueSize > void BtQueue< T, MaxQueueSize >::Empty()
{
	m_pHead = BtNull;
	m_pBack = BtNull;
	m_nItems = 0;
	m_nCurrentItem = 0;
}
//////////////////////////////////////////////////////////////////////////
// PushFront

template< class T, int MaxQueueSize > T* BtQueue< T, MaxQueueSize >::PushFront(const T& Data)
{
#ifdef _DEBUG
	if(m_nItems > MaxQueueSize - 1)
	{
		BtAssert(0);
	}
#endif // _DEBUG

	// Get next item in queue
	BtQueueItem& item = m_queue[m_nCurrentItem++];

	// Wrap current item back round
	if(m_nCurrentItem >= MaxQueueSize)
	{
		m_nCurrentItem = 0;
	}

	item.m_pNext = BtNull;
	item.m_item = Data;

	// Push to list.
	if(m_pBack == BtNull)
	{
		// We are the first item to be Pushed.
		m_pHead = m_pBack = &item;
	}
	else
	{
		// Item at back of queue, make us its next, and then us the back
		m_pHead->m_pNext = &item;
		m_pHead = &item;
	}

	// Increment items in queue.
	++m_nItems;

	return &item.m_item;
}

//////////////////////////////////////////////////////////////////////////
// Push

template< class T, int MaxQueueSize > T* BtQueue< T, MaxQueueSize >::Push( const T& Data )
{
#ifdef _DEBUG
	if( m_nItems > MaxQueueSize-1 )
	{
		BtAssert(0);
	}
#endif // _DEBUG

	// Get next item in queue
	BtQueueItem& item = m_queue[m_nCurrentItem++];

	// Wrap current item back round
	if ( m_nCurrentItem >= MaxQueueSize )
	{
		m_nCurrentItem = 0;
	}

	item.m_pNext = BtNull;
	item.m_item = Data;

	// Push to list.
	if ( m_pBack == BtNull )
	{
		// We are the first item to be Pushed.
		m_pHead = m_pBack = &item;
	}
	else
	{
		// Item at back of queue, make us its next, and then us the back
		m_pBack->m_pNext = &item;
		m_pBack = &item;
	}

	// Increment items in queue.
	++m_nItems;

	return &item.m_item;
}

//////////////////////////////////////////////////////////////////////////
// Pop

template< class T, int MaxQueueSize > T BtQueue< T, MaxQueueSize >::Pop()
{
#ifdef _DEBUG
	if( m_nItems == 0 )
	{
		BtAssert(0);
	}
#endif // _DEBUG
	
	BtAssert( m_pHead );

	// Get the head of the queue
    const T& RetData = m_pHead->m_item;

	// Set the head of the queue to the next item
	m_pHead = m_pHead->m_pNext;

	// If the queue has been emptied then set the back of the queue to empty
	if ( m_pHead == BtNull )
	{
		m_pBack = BtNull;
	}

	//
	--m_nItems;

	return RetData;
}

//////////////////////////////////////////////////////////////////////////
// Peek

template< class T, int MaxQueueSize > T& BtQueue< T, MaxQueueSize >::Peek()
{
#ifdef _DEBUG
	if( m_nItems == 0 )
	{
		BtAssert(0);
	}
#endif // _DEBUG
	
	BtAssert( m_pHead );

	// Get the head of the queue
	T& head = m_pHead->m_item;

	// Return the data
	return head;
}

//////////////////////////////////////////////////////////////////////////
// Peek

template< class T, int MaxQueueSize > T& BtQueue< T, MaxQueueSize >::Peek( BtU32 iIndex )
{
#ifdef _DEBUG
	if( m_nItems == 0 )
	{
		BtAssert(0);
	}
#endif // _DEBUG

	BtAssert( m_pHead );

	BtQueueItem* pQueue = m_pHead;

	BtU32 nCount = 0;

	while( pQueue )
	{
		if( nCount == iIndex )
		{
			// Get the head of the queue
			T& current = pQueue->m_item;

			// Return the head of the queue
			return current;
		}

		// Increment the queue position
		++nCount;

		// Go to the next place in the queue
		pQueue = pQueue->m_pNext;
	}

	// Get the head of the queue
	T& head = m_pHead->m_item;

	// Return the data
	return head;
}


//////////////////////////////////////////////////////////////////////////
// GetItemCount

template< class T, int MaxQueueSize > BtU32 BtQueue< T, MaxQueueSize >::GetItemCount() const
{
	return m_nItems;
}

//////////////////////////////////////////////////////////////////////////
// GetMaxItems

template< class T, int MaxQueueSize > BtU32 BtQueue< T, MaxQueueSize >::GetMaxItems() const
{
	return MaxQueueSize;
}

//////////////////////////////////////////////////////////////////////////
// IsRoom

template< class T, int MaxQueueSize > BtBool BtQueue< T, MaxQueueSize >::IsRoom() const
{
	if( m_nItems < MaxQueueSize - 1 )
	{
		return BtTrue;
	}
	return BtFalse;
}

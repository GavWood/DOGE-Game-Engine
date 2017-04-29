////////////////////////////////////////////////////////////////////////////////
// BtLinkedList.h

#pragma once
#include "BtTypes.h"

////////////////////////////////////////////////////////////////////////////////
// BtLinkedListItem

template< typename _T > class BtLinkedListItem
{
public:

	BtLinkedListItem();

	_T*					pPrev() const;
	_T*					pNext() const;

	void				pPrev( _T* );
	void				pNext( _T* );

private:

	// Data members
	_T*					pPrev_;
	_T*					pNext_;
};

////////////////////////////////////////////////////////////////////////////////
// Inlines

template< typename _T > inline BtLinkedListItem< _T >::BtLinkedListItem()
{
	pPrev_ = BtNull;
	pNext_ = BtNull;
}

template< typename _T > inline _T* BtLinkedListItem< _T >::pPrev() const
{
	return pPrev_;
}

template< typename _T > inline _T* BtLinkedListItem< _T >::pNext() const
{
	return pNext_;
}

template< typename _T > inline void BtLinkedListItem< _T >::pNext( _T* pItem )
{
	pNext_ = pItem;
}

template< typename _T > inline void BtLinkedListItem< _T >::pPrev( _T* pItem )
{
	pPrev_ = pItem;
}

////////////////////////////////////////////////////////////////////////////////
// BtLinkedList

template< typename _T > class BtLinkedList
{
public:

	BtLinkedList();
	~BtLinkedList();

	// Item access
	_T*					pHead() const;
	_T*					pTail() const;

	// Is the list empty?
	BtBool				IsEmpty() const;

	// Add to the list
	void				AddHead( _T* pItem );
	void				AddTail( _T* pItem );
	void				InsertAfter( _T* pItem, _T* pNewItem );
	void				InsertBefore( _T* pItem, _T* pNewItem );

	// Remove from the list
	void				Remove( _T* pItem, BtBool bDeleteItem = BtFalse );
	void				Empty( BtBool bDeleteItems = BtFalse );

private:

	// Members
	_T*					m_pHead;
	_T*					m_pTail;
};

////////////////////////////////////////////////////////////////////////////////
// Inlines

template< typename _T > inline BtLinkedList< _T >::BtLinkedList()
{
	m_pHead = BtNull;
	m_pTail = BtNull;
}

template< typename _T > inline BtBool BtLinkedList< _T >::IsEmpty() const
{
	if( m_pHead == BtNull )
	{
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

template< typename _T > inline BtLinkedList< _T >::~BtLinkedList()
{
	if( IsEmpty() == BtFalse )
	{
		int a=0;
		a++;
	}
}

template< typename _T > inline _T* BtLinkedList< _T >::pHead() const
{
	return m_pHead;
}

template< typename _T > inline _T* BtLinkedList< _T >::pTail() const
{
	return m_pTail;
}

template< typename _T > inline void BtLinkedList< _T >::AddHead( _T* pItem )
{
	if( m_pHead == BtNull )
	{
		m_pHead = pItem;
		m_pTail = pItem;
		pItem->pPrev( BtNull );
		pItem->pNext( BtNull );
	}
	else
	{
		InsertBefore( pHead(), pItem );
	}
}

template< typename _T > inline void BtLinkedList< _T >::AddTail( _T* pItem )
{
	if( m_pTail == BtNull )
	{
		AddHead( pItem );
	}
	else
	{
		InsertAfter( m_pTail, pItem );
	}
}

template< typename _T > inline void BtLinkedList< _T >::InsertAfter( _T* pItem, _T* pNewItem )
{
	pNewItem->pPrev( pItem );
	pNewItem->pNext( pItem->pNext() );

	if( pItem->pNext() == BtNull )
	{
		m_pTail = pNewItem;
	}
	else
	{
		pItem->pNext()->pPrev( pNewItem );
	}

	pItem->pNext( pNewItem );
}

template< typename _T > inline void BtLinkedList< _T >::InsertBefore( _T* pItem, _T* pNewItem )
{
	pNewItem->pPrev( pItem->pPrev() );
	pNewItem->pNext( pItem );

	if( pItem->pPrev() == BtNull )
	{
		m_pHead = pNewItem;
	}
	else
	{
		pItem->pPrev()->pNext( pNewItem );
	}

	pItem->pPrev( pNewItem );
}

template< typename _T > inline void BtLinkedList< _T >::Remove( _T* pItem, BtBool bDeleteItem )
{
	if( pItem->pPrev() == BtNull )
	{
		// Removing head item
		m_pHead = pItem->pNext();
	}
	else
	{
		pItem->pPrev()->pNext( pItem->pNext() );
	}

	if( pItem->pNext() == BtNull )
	{
		// Removing tail item
		m_pTail = pItem->pPrev();
	}
	else
	{
		pItem->pNext()->pPrev( pItem->pPrev() );
	}

	if( bDeleteItem == BtTrue )
	{
		delete pItem;
	}
}

template< typename _T > inline void BtLinkedList< _T >::Empty(BtBool bDeleteItems)
{
	_T* pCurrent = m_pHead;

	while (pCurrent != BtNull)
	{
		_T* pNext = pCurrent->pNext();
		Remove(pCurrent, bDeleteItems);
		pCurrent = pNext;
	}
}

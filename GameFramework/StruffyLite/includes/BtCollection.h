////////////////////////////////////////////////////////////////////////////////
// BtCollection.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BtBase.h"
#include "BtArray.h"
#include "BtLinkedList.h"

////////////////////////////////////////////////////////////////////////////////
// BtCollectionElement

template <class T> class BtCollectionElement : public BtLinkedListItem< BtCollectionElement<T> >
{
public:
	
	T*													GetElement();

//private:

	//template <class T, BtU32 maxItems > friend class BtCollectionIterator;

	T													m_element;
};

////////////////////////////////////////////////////////////////////////////////
// GetElemeent

template <class T> T* BtCollectionElement<T>::GetElement()
{
	return &m_element;
}

// Class definition
template <class T, BtU32 maxItems> class BtCollection
{
public:

	// Constructor
	BtCollection();

	// Destructor
	~BtCollection();

	// Public functions
	void												Empty();
	T*													GetHead();
	T*													GetTail();
	T*													AddHead();
	T*													AddTail();
	void												AddHead( T elem );
	void												AddTail( T elem );
	T*													InsertBefore( BtCollectionElement<T> *pItem );
	T*													InsertAfter( BtCollectionElement<T> *pItem );

	// Accessors
	BtU32												GetNumElements();
	BtU32												GetMaxElements();

//private:

//	template <class T, BtU32 maxItems > friend class BtCollectionIterator;

	void												DeleteElement( BtCollectionElement<T> *pToDelete );
	void												DeleteElement( T *pToDelete );
	BtCollectionElement<T>*								GetFreeElement();

	BtArray< BtCollectionElement<T>, maxItems>			m_elements;
	BtLinkedList< BtCollectionElement<T> >				m_free;
	BtLinkedList< BtCollectionElement<T> >				m_allocated;
	BtU32												m_numElements;
};

////////////////////////////////////////////////////////////////////////////////
// BtCollection

template <class T, BtU32 maxItems> BtCollection<T, maxItems>::BtCollection()
{
	// Empty first
	Empty();
}

////////////////////////////////////////////////////////////////////////////////
// BtCollection

template <class T, BtU32 maxItems> BtCollection<T, maxItems>::~BtCollection()
{
	// Free both the arrays
	m_free.Empty( BtFalse );
	m_allocated.Empty( BtFalse );

	// Set the number of elements in the array
	m_elements.SetNumItems( 0 );
}

////////////////////////////////////////////////////////////////////////////////
// Empty

template <class T, BtU32 maxItems> void BtCollection<T, maxItems>::Empty()
{
	// Free both the arrays
	m_free.Empty( BtFalse );
	m_allocated.Empty( BtFalse );

	// Set the number of elements
	m_numElements = 0;

	// Set the number of elements in the array
	m_elements.SetNumItems( maxItems );

	// Then fill the array
	for( BtU32 i=0; i<maxItems; i++ )
	{
		// Cache each element from the array
		BtCollectionElement<T>* pElement = &m_elements[i];

		// Add to the linked list
		m_free.AddHead( pElement );
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetFreeElement

template <class T, BtU32 maxItems> BtCollectionElement<T>* BtCollection<T, maxItems>::GetFreeElement()
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = m_free.pHead();

	// Remove the item from the free queue
	m_free.Remove( pFreeHead, BtFalse );

	// Increment then number of elements
	++m_numElements;

	// Return the pointer to the item
	return pFreeHead;
}

////////////////////////////////////////////////////////////////////////////////
// GetHead

template <class T, BtU32 maxItems> T* BtCollection<T, maxItems>::GetHead()
{
	// Return the pointer to the item
	return m_allocated.pHead()->GetElement();
}

////////////////////////////////////////////////////////////////////////////////
// GetTail

template <class T, BtU32 maxItems> T* BtCollection<T, maxItems>::GetTail()
{
	// Return the pointer to the item
	return m_allocated.pTail()->GetElement();
}

////////////////////////////////////////////////////////////////////////////////
// AddHead

template <class T, BtU32 maxItems> T* BtCollection<T, maxItems>::AddHead()
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = GetFreeElement();

	// Add to the allocated array
	m_allocated.AddHead( pFreeHead );

	// Get the element inside this list item
	T* pElement = pFreeHead->GetElement();

	// Return the pointer to the item
	return pElement;
}

////////////////////////////////////////////////////////////////////////////////
// AddHead

template <class T, BtU32 maxItems> void BtCollection<T, maxItems>::AddHead( T elem )
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = GetFreeElement();

	// Add to the allocated array
	m_allocated.AddHead( pFreeHead );

	// Get the element inside this list item
	T* pElement = pFreeHead->GetElement();

	// Return the pointer to the item
	*pElement = elem;
}

////////////////////////////////////////////////////////////////////////////////
// InsertAfter

template <class T, BtU32 maxItems> T* BtCollection<T, maxItems>::InsertAfter( BtCollectionElement<T> *pItem )
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = GetFreeElement();

	// Add to the allocated array
	m_allocated.InsertAfter( pItem, pFreeHead );

	// Get the element inside this list item
	T* pElement = pFreeHead->GetElement();

	// Return the pointer to the item
	return pElement;
}

////////////////////////////////////////////////////////////////////////////////
// InsertBefore

template <class T, BtU32 maxItems> T* BtCollection<T, maxItems>::InsertBefore( BtCollectionElement<T> *pItem )
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = GetFreeElement();

	// Add to the allocated array
	m_allocated.InsertBefore( pItem, pFreeHead );

	// Get the element inside this list item
	T* pElement = pFreeHead->GetElement();

	// Return the pointer to the item
	return pElement;
}

////////////////////////////////////////////////////////////////////////////////
// AddTail

template <class T, BtU32 maxItems> T* BtCollection<T, maxItems>::AddTail()
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = GetFreeElement();

	// Add to the allocated array
	m_allocated.AddTail( pFreeHead );

	// Get the element inside this list item
	T* pElement = pFreeHead->GetElement();

	// Return the pointer to the item
	return pElement;
}

////////////////////////////////////////////////////////////////////////////////
// AddTail

template <class T, BtU32 maxItems> void BtCollection<T, maxItems>::AddTail( T elem )
{
	// Cache the free head
	BtCollectionElement<T>* pFreeHead = GetFreeElement();

	// Add to the allocated array
	m_allocated.AddTail( pFreeHead );

	// Get the element inside this list item
	T* pElement = pFreeHead->GetElement();

	// Return the pointer to the item
	*pElement = elem;
}

////////////////////////////////////////////////////////////////////////////////
// DeleteCurrentElement

template <class T, BtU32 maxItems> void BtCollection<T, maxItems>::DeleteElement( BtCollectionElement<T> *pToDelete )
{
	BtAssert( m_numElements > 0 );
	
	// Decrement the number of elements
	--m_numElements;

	// Remove the current item for the allocated list
	m_allocated.Remove( pToDelete, BtFalse );

	// Add this item to the free list
	m_free.AddTail( pToDelete );
}

////////////////////////////////////////////////////////////////////////////////
// DeleteCurrentElement

template <class T, BtU32 maxItems> void BtCollection<T, maxItems>::DeleteElement( T *pToDelete )
{
	BtCollectionElement<T> *pList = m_allocated.pHead();

	while( pList )
	{
		if( pList->GetElement() == pToDelete )
		{
			DeleteElement( pList );
		}

		// Go to the next renderable
		pList = pList->pNext();
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNumElements

template <class T, BtU32 maxItems> BtU32 BtCollection<T, maxItems>::GetNumElements()
{
	return m_numElements;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaxElements

template <class T, BtU32 maxItems> BtU32 BtCollection<T, maxItems>::GetMaxElements()
{
	return maxItems;
}

template <class T, BtU32 maxItems > class BtCollectionIterator
{
public:

	// Constructor
	BtCollectionIterator();
	BtCollectionIterator( BtCollection< T, maxItems > &list );

	// Public functions
	T*													GetFirstElement();
	T*													GetTailElement();
	T*													GetNextElement();
	T*													GetPreviousElement();
	T*													GetCurrentElement();
	T*													InsertAfter();
	T*													InsertBefore();
	static void											SwapElements( BtCollectionIterator< T, maxItems> &iterator1, BtCollectionIterator< T, maxItems> &iterator2 );
	void												DeleteElement();

//private:

//	template <class T, BtU32 maxItems > friend class BtCollectionIterator;

	BtCollection< T, maxItems >							*m_pList;
	BtCollectionElement< T >							*m_pCurrent;
	BtCollectionElement< T >							*m_pNext;
	BtCollectionElement< T >							*m_pPrevious;
};

////////////////////////////////////////////////////////////////////////////////
// Constructor

template<class T, BtU32 maxItems > BtCollectionIterator<T, maxItems>::BtCollectionIterator()
{
	m_pCurrent = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

template<class T, BtU32 maxItems > BtCollectionIterator<T, maxItems>::BtCollectionIterator( BtCollection< T, maxItems > &list )
{
	m_pCurrent = BtNull;
	m_pList = &list;
}

////////////////////////////////////////////////////////////////////////////////
// InsertBefore

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::InsertBefore()
{
	T* pElement = m_pList->InsertBefore( m_pCurrent );

	return pElement;
}

////////////////////////////////////////////////////////////////////////////////
// InsertAfter

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::InsertAfter()
{
	T* pElement = m_pList->InsertAfter( m_pCurrent );

	return pElement;
}

////////////////////////////////////////////////////////////////////////////////
// GetCurrentElement

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::GetCurrentElement()
{
	if( m_pCurrent != BtNull )
	{
		// Get the element inside this list item
		T* pElement = m_pCurrent->GetElement();

		// Return the pointer to the item
		return pElement;
	}
	else
	{
		return BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetFirstElement

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::GetFirstElement()
{
	// Cache the allocated head
	m_pCurrent = m_pList->m_allocated.pHead();

	if( m_pCurrent != BtNull )
	{
		// Get the next element
		m_pNext = m_pCurrent->pNext();

		// Get the previous element
		m_pPrevious = m_pCurrent->pNext();

		// Get the element inside this list item
		T* pElement = m_pCurrent->GetElement();

		// Return the pointer to the item
		return pElement;
	}
	else
	{
		return BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetTailElement

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::GetTailElement()
{
	// Cache the allocated head
	m_pCurrent = m_pList->m_allocated.pTail();

	if( m_pCurrent != BtNull )
	{
		// Get the next element
		m_pNext = m_pCurrent->pNext();

		// Get the previous element
		m_pPrevious = m_pCurrent->pPrev();

		// Get the element inside this list item
		T* pElement = m_pCurrent->GetElement();

		// Return the pointer to the item
		return pElement;
	}
	else
	{
		return BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNextElement

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::GetNextElement()
{
	// Get the next element
	m_pCurrent = m_pNext;

	if( m_pCurrent != BtNull )
	{
		// Get the element inside this list item
		T* pElement = m_pCurrent->GetElement();

		// Set the next pointer
		m_pNext = m_pCurrent->pNext();

		// Get the previous element
		m_pPrevious = m_pCurrent->pPrev();

		// Return the pointer to the item
		return pElement;
	}
	else
	{
		return BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetPreviousElement

template <class T, BtU32 maxItems> T* BtCollectionIterator<T, maxItems>::GetPreviousElement()
{
	// Get the next element
	m_pCurrent = m_pPrevious;

	if( m_pCurrent != BtNull )
	{
		// Get the element inside this list item
		T* pElement = m_pCurrent->GetElement();

		// Set the next pointer
		m_pPrevious = m_pCurrent->pPrev();

		// Return the pointer to the item
		return pElement;
	}
	else
	{
		return BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// DeleteElement

template <class T, BtU32 maxItems> void BtCollectionIterator<T, maxItems>::DeleteElement()
{
	// Validate the current element
	BtAssert( m_pCurrent != BtNull );

	// Delete element
	m_pList->DeleteElement( m_pCurrent );
}

////////////////////////////////////////////////////////////////////////////////
// SwapElements

template <class T, BtU32 maxItems> void BtCollectionIterator<T, maxItems>::SwapElements( BtCollectionIterator< T, maxItems> &iterator1, BtCollectionIterator< T, maxItems> &iterator2 )
{
	BtSwap( iterator1.m_pCurrent->m_element, iterator2.m_pCurrent->m_element );
}

////////////////////////////////////////////////////////////////////////////////
// BtArray.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BtPrint.h"
#include "BtBase.h"
#include "BtMemory.h"
#include "RdRandom.h"

// Class definition
template <class T, BtU32 maxItems> class BtArray
{
public:

		BtArray();

		void								Empty();
		T*									Add();
		T*									Add( const T& ele );
		void								Delete( T* pEle );

		void								SetNumItems( BtU32 numItems );
		BtU32								GetNumItems() const;
		BtU32								GetMaxItems() const;
		BtU32								GetElementSize();
		T&									GetElement( BtU32 item );
		void								Randomise();

		BtU32								GetLastIndex() const;
		T&									GetLast();
		BtBool								IsEmpty() const;
		BtBool								IsRoom() const;

		T&									operator []( BtU32 i );
		const T&							operator []( BtU32 i ) const;

private:

		BtU32								m_nItems;
		BtU32								m_nSizeElement;
		T									m_array[maxItems];
		BtU32								m_random[maxItems];
};

////////////////////////////////////////////////////////////////////////////////
// Constructor

template <class T, BtU32 maxItems> BtArray<T, maxItems>::BtArray()
{
	m_nItems = 0;
	m_nSizeElement = sizeof(T);
}

////////////////////////////////////////////////////////////////////////////////
// Empty

template <class T, BtU32 maxItems> void BtArray<T, maxItems>::Empty()
{
	m_nItems = 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetElement

template <class T, BtU32 maxItems> T& BtArray<T, maxItems>::GetElement( BtU32 iIndex )
{
	BtAssert( iIndex < maxItems );

	if( iIndex >= maxItems )
	{
		BtPrint( "BtArray access[] Index %d exceeds number of items %d.", iIndex, maxItems );
	}
	return m_array[ iIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// Array access operator

template <class T, BtU32 maxItems> T& BtArray<T, maxItems>::operator []( BtU32 iIndex )
{
	BtAssert( iIndex < maxItems );

	if( iIndex >= maxItems )
	{
		BtPrint( "BtArray access[] Index %d exceeds number of items %d.", iIndex, maxItems );
	}

	return m_array[ iIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// Array access operator

template <class T, BtU32 maxItems> const T& BtArray<T, maxItems>::operator []( BtU32 iIndex ) const
{
	BtAssert( iIndex < maxItems );

	if( iIndex >= maxItems )
	{
		BtPrint( "BtArray access[] Index %d exceeds number of items %d.", iIndex, maxItems );
	}
	return m_array[ iIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// Add

template <class T, BtU32 maxItems> T* BtArray<T, maxItems>::Add()
{
	BtAssert( m_nItems < maxItems )

	// Cache the element
	T& element = m_array[m_nItems];

	// Increment the number of items
	m_nItems++;
	
	// Return the pointer to the item
	return &element;
}

////////////////////////////////////////////////////////////////////////////////
// Delete

template <class T, BtU32 maxItems> void BtArray<T, maxItems>::Delete( T* pEle )
{
	T* pFound = BtNull;

	for( BtU32 i=0; i<m_nItems; i++ )
	{
		// Get a point to the current node
		T* pCurrent = &m_array[i];

		if( pEle == pCurrent )
		{
			pFound = pCurrent;
		}
	}

	if( pFound != BtNull )
	{
		// Cache the element
		T& lastElement = m_array[m_nItems - 1];

		// Set the element
		if( ( void*)pFound != (void*)&lastElement )
		{
			BtMemory::Copy( (void*)pFound, (void*)&lastElement, sizeof(T) );
		}

		// Decrement the number of items
		m_nItems--;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Add

template <class T, BtU32 maxItems> T* BtArray<T, maxItems>::Add( const T& ele )
{
	if( m_nItems < maxItems )
	{
		// Cache the element
		memcpy( &m_array[m_nItems], &ele, sizeof(T) );

		// Increment the number of items
		m_nItems++;

		// Return the pointer to the item
		return &m_array[m_nItems - 1];
	}
	else
	{
		return NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetNumItems

template <class T, BtU32 maxItems> void BtArray<T, maxItems>::SetNumItems( BtU32 numItems )
{
	m_nItems = numItems;
}

////////////////////////////////////////////////////////////////////////////////
// GetLastIndex

template <class T, BtU32 maxItems> BtU32 BtArray<T, maxItems>::GetLastIndex() const
{
	// Return the number of items
	return m_nItems - 1;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

template <class T, BtU32 maxItems> BtU32 BtArray<T, maxItems>::GetNumItems() const
{
	// Return the number of items
	return m_nItems;
}

////////////////////////////////////////////////////////////////////////////////
// GetLast

template <class T, BtU32 maxItems> T& BtArray<T, maxItems>::GetLast()
{
	BtAssert( m_nItems < maxItems );

	BtU32 iIndex = m_nItems - 1;

	return m_array[ iIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// IsEmpty

template <class T, BtU32 maxItems> BtBool BtArray<T, maxItems>::IsEmpty() const
{
	// Return the number of items
	if( m_nItems == 0 )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsRoom

template <class T, BtU32 maxItems> BtBool BtArray<T, maxItems>::IsRoom() const
{
	// Return the number of items
	if( m_nItems < maxItems - 1 )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaxItems

template <class T, BtU32 maxItems> BtU32 BtArray<T, maxItems>::GetMaxItems() const
{
	// Return the maximum number of items
	return maxItems;
}

///////////////////////////////////////////////////////////////////////////////
// GetElementSize

template <class T, BtU32 maxItems> BtU32 BtArray<T, maxItems>::GetElementSize()
{
	// Return the size
	return m_nSizeElement;
}

///////////////////////////////////////////////////////////////////////////////
// Randomise

template <class T, BtU32 maxItems> void BtArray<T, maxItems>::Randomise()
{
	for( BtU32 iRandomIndex=0; iRandomIndex<m_nItems; iRandomIndex++ )
	{
		m_random[iRandomIndex] = RdRandom::GetNumber( 0, 65535 );
	}

	for( BtU32 i=0; i<m_nItems; i++ )
	{
		for( BtU32 j=0; j<m_nItems; j++ )
		{
			if( i != j )
			{
				if( m_random[i] < m_random[j] )
				{
					BtSwap( m_random[i], m_random[j] );
					BtSwap( m_array[i], m_array[j] );
				}
			}
		}
	}
}

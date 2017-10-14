////////////////////////////////////////////////////////////////////////////////
// MCArray.h

// Include guard
#ifndef __MCArray_h__
#define __MCArray_h__

#include "BtTypes.h"
#include "BtBase.h"
#include "BtMemory.h"
#include "RdRandom.h"
#include "ErrorLog.h"
#include <vector>

// Class definition
template <class T, BtU32 nMaxItems> class McArray
{
public:

		McArray();

		void								Empty();
		T*									Add();
		void								Add( T ele );

		void								SetNumItems( BtU32 numItems );
		BtU32								GetNumItems() const;
		BtU32								GetMaxItems() const;
		BtU32								GetElementSize();

		T&									operator []( BtU32 i );
		const T&							operator []( BtU32 i ) const;

private:

		BtU32								m_nItems;
		BtU32								m_nSize;
		std::vector<T>						m_array;
};

////////////////////////////////////////////////////////////////////////////////
// Constructor

template <class T, BtU32 nMaxItems> McArray<T, nMaxItems>::McArray()
{
	m_nItems = 0;
	m_nSize = sizeof(T);
}

////////////////////////////////////////////////////////////////////////////////
// Empty

template <class T, BtU32 nMaxItems> void McArray<T, nMaxItems>::Empty()
{
	m_nItems = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Array access operator

template <class T, BtU32 nMaxItems> T& McArray<T, nMaxItems>::operator []( BtU32 iIndex )
{
	if( iIndex >= nMaxItems )
	{
		ErrorLog::Fatal_Printf( "McArray[]",
								"Index %d exceeds number of items %d.", iIndex, nMaxItems );
	}

	return m_array[ iIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// Array access operator

template <class T, BtU32 nMaxItems> const T& McArray<T, nMaxItems>::operator []( BtU32 iIndex ) const
{
	if( iIndex >= nMaxItems )
	{
		ErrorLog::Fatal_Printf( "McArray[]",
			"Index %d exceeds number of items %d.", iIndex, nMaxItems );
	}

	return m_array[ iIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// Add

template <class T, BtU32 nMaxItems> T* McArray<T, nMaxItems>::Add()
{
#ifdef STRUFFY_DEBUG
	if( m_nItems >= nMaxItems )
	{
		ErrorLog::Fatal_Printf( "McArray full.\n" );
	}
#endif // STRUFFY_DEBUG

	// Cache the element
	T& element = m_array[m_nItems];

	// Increment the number of items
	m_nItems++;
	
	// Return the pointer to the item
	return &element;
}

////////////////////////////////////////////////////////////////////////////////
// Add

template <class T, BtU32 nMaxItems> void McArray<T, nMaxItems>::Add( T ele )
{
#ifdef STRUFFY_DEBUG
	if( m_nItems >= nMaxItems )
	{
		ErrorLog::Fatal_Printf( "McArray full.\n" );
	}
#endif // STRUFFY_DEBUG

	m_array.resize( m_nItems + 1 );

	// Cache the element
	m_array[m_nItems] = ele;

	// Increment the number of items
	m_nItems++;
}

////////////////////////////////////////////////////////////////////////////////
// SetNumItems

template <class T, BtU32 nMaxItems> void McArray<T, nMaxItems>::SetNumItems( BtU32 numItems )
{
	m_nItems = numItems;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

template <class T, BtU32 nMaxItems> BtU32 McArray<T, nMaxItems>::GetNumItems() const
{
	// Return the number of items
	return m_nItems;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaxItems

template <class T, BtU32 nMaxItems> BtU32 McArray<T, nMaxItems>::GetMaxItems() const
{
	// Return the maximum number of items
	return nMaxItems;
}

///////////////////////////////////////////////////////////////////////////////
// GetElementSize

template <class T, BtU32 nMaxItems> BtU32 McArray<T, nMaxItems>::GetElementSize()
{
	// Return the size
	return m_nSize;
}

#endif // __McArray_h__

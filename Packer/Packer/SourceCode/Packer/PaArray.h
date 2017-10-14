////////////////////////////////////////////////////////////////////////////////
// PaArray.h

// Include guard
#ifndef __PaArray_h__
#define __PaArray_h__

#include "BtTypes.h"
#include "ErrorLog.h"
#include "PaMemoryResource.h"

// Class definition
template <class T, BtU32 nMaxItems> class PaArray
{
public:

		PaArray();

		void								Empty();
		T&									Add();

		void								Read( PaMemoryResource& memoryResource );
		void								Write( PaMemoryResource& memoryResource );

		BtU32								nItems();
		BtU32								nSize();

		const T&							operator []( BtU32 i ) const;

private:

		BtU32								m_nItems;
		BtU32								m_nSize;
		T									m_array[nMaxItems];
};

////////////////////////////////////////////////////////////////////////////////
// Constructor

template <class T, BtU32 nMaxItems> PaArray<T, nMaxItems>::PaArray()
{
	m_nItems = 0;
	m_nSize = sizeof(T);
}

////////////////////////////////////////////////////////////////////////////////
// Empty

template <class T, BtU32 nMaxItems> void PaArray<T, nMaxItems>::Empty()
{
	m_nItems = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Array access operator

template <class T, BtU32 nMaxItems> const T& PaArray<T, nMaxItems>::operator []( BtU32 i ) const
{
	if( i >= nMaxItems )
	{
		ErrorLog::Fatal_Printf( "PaArray out of bounds access %d out of %d.\n", i, nMaxItems  );
	}

	BtU32 nIndex = (m_nItems - 1) - i;

	return m_array[ nIndex ];
}

////////////////////////////////////////////////////////////////////////////////
// Add

template <class T, BtU32 nMaxItems> T& PaArray<T, nMaxItems>::Add()
{
#ifdef SUSI_DEBUG
	if( m_nItems >= nMaxItems )
	{
		ErrorLog::Fatal_Printf( "PaArray full.\n" );
	}
#endif // SUSI_DEBUG

	// Get the next item
	T& pushedElement = m_array[m_nItems];

	// Increment the number of items
	m_nItems++;
	
	// Return the pointer to the next item
	return pushedElement;
}

////////////////////////////////////////////////////////////////////////////////
// Read

template <class T, BtU32 nMaxItems> void PaArray<T, nMaxItems>::Read( PaMemoryResource& memoryResource )
{
	memoryResource >> m_nItems;
	memoryResource >> m_nSize;

	if( m_nItems > 0 )
	{
		for( BtU32 nIndex=0; nIndex<m_nItems; nIndex++ )
		{
			memoryResource >> m_array[nIndex];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Write

template <class T, BtU32 nMaxItems> void PaArray<T, nMaxItems>::Write( PaMemoryResource& memoryResource )
{
	memoryResource << m_nItems;
	memoryResource << m_nSize;

	if( m_nItems > 0 )
	{
		for( BtU32 nIndex=0; nIndex<m_nItems; nIndex++ )
		{
			memoryResource << m_array[nIndex];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// nItems

template <class T, BtU32 nMaxItems> BtU32 PaArray<T, nMaxItems>::nItems()
{
	// Return the number of items
	return m_nItems;
}

///////////////////////////////////////////////////////////////////////////////
// nSize

template <class T, BtU32 nMaxItems> BtU32 PaArray<T, nMaxItems>::nSize()
{
	// Return the size
	return m_nSize;
}

#endif // __PaArray_h__

////////////////////////////////////////////////////////////////////////////////
// PaMemoryResource

#pragma once
#include "BtTypes.h"
#include "BtArray.h"
#include "BtSingleton.h"
#include "BtMemory.h"

enum PMR_Usage
{
	PMR_Read,
	PMR_Write,
};

class PaMemoryItem
{
public:

	PaMemoryItem( BtU8* pItem, BtU32 nSize )
	{
		m_pItem = pItem;
		m_nSize = nSize;
	}

private:
	friend class PaMemoryResource;

	BtU8* m_pItem;
	BtU32 m_nSize;
};

class PaMemoryResource
{
public:

	PaMemoryResource( PMR_Usage eUsage, BtU32 nSize = 512 * 1024 );
	~PaMemoryResource();

	BtBool							Load( BtChar* pFilename );
	void							Save( BtChar* pFilename );
	void							Append( BtChar* pFilename );

	BtU32							GetCurrentSize();
	BtU32							Tell();

	// Read
	void							operator >> ( BtU32 &nValue );
	void							operator >> ( BtS32 &nValue );
	void							operator >> ( BtFloat &fValue );
	template <typename T> void		operator >> ( T& );

	// Write
	void							operator << ( const PaMemoryItem& item );
	void							operator << ( BtU32 nValue );
	void							operator << ( BtS32 nValue );
	void							operator << ( BtFloat fValue );
	template <typename T> void		operator << ( T& );

	BtU8*							pCurrent();
	BtU8*							GetBase();

private:

	void							updateBufferSize( BtU32 nLength );

	// Members
	BtU8*							m_pBaseAddress;
	BtU8*							m_pCurrentAddress;
	
	BtU32							m_nMemorySize;

	PMR_Usage						m_usage;
};

////////////////////////////////////////////////////////////////////////////////
// operator >>

template< typename T > inline void PaMemoryResource::operator >> ( T& Data )
{
	BtU32 sizeOfTemplate = sizeof(T);
	BtAssert( m_usage == PMR_Read );
	BtMemory::Copy( &Data, m_pCurrentAddress, sizeOfTemplate );
	m_pCurrentAddress+= sizeOfTemplate;
}

////////////////////////////////////////////////////////////////////////////////
// operator >>

inline void PaMemoryResource::operator >> (BtU32 &nValue)
{
	BtAssert( m_usage == PMR_Read );
	nValue = *(BtU32*) m_pCurrentAddress;
	m_pCurrentAddress+= sizeof(BtU32);
}

////////////////////////////////////////////////////////////////////////////////
// operator >>

inline void PaMemoryResource::operator >> (BtS32 &nValue)
{
	BtAssert( m_usage == PMR_Read );
	nValue = *(BtS32*) m_pCurrentAddress;
	m_pCurrentAddress+= sizeof(BtS32);
}

////////////////////////////////////////////////////////////////////////////////
// operator >>

inline void PaMemoryResource::operator >> (BtFloat &fValue)
{
	BtAssert( m_usage == PMR_Read );
	fValue = *(BtFloat*) m_pCurrentAddress;
	m_pCurrentAddress+= sizeof(BtFloat);
}

////////////////////////////////////////////////////////////////////////////////
// operator <<

inline void PaMemoryResource::operator << (BtS32 nValue)
{
	BtAssert( m_usage == PMR_Write );

	updateBufferSize( sizeof(BtS32) );

	BtS32& Value = *(BtS32*) m_pCurrentAddress;

	Value = nValue;

	m_pCurrentAddress+= sizeof(BtS32);
}

////////////////////////////////////////////////////////////////////////////////
// operator <<

inline void PaMemoryResource::operator << (BtU32 nValue)
{
	BtAssert( m_usage == PMR_Write );

	updateBufferSize( sizeof(BtU32) );

	BtU32& Value = *(BtU32*) m_pCurrentAddress;

	Value = nValue;

	m_pCurrentAddress+= sizeof(BtU32);
}

////////////////////////////////////////////////////////////////////////////////
// operator <<

inline void PaMemoryResource::operator << (BtFloat fValue)
{
	BtAssert( m_usage == PMR_Write );

	updateBufferSize( sizeof(BtFloat) );

	BtFloat& Value = *(BtFloat*) m_pCurrentAddress;
		
	Value = fValue;

	m_pCurrentAddress+= sizeof(BtFloat);
}

////////////////////////////////////////////////////////////////////////////////
// operator <<

template< typename T > inline void PaMemoryResource::operator << ( T& data )
{
	BtAssert( m_usage == PMR_Write );

	BtU32 sizeOfTemplate = sizeof(T);

	updateBufferSize( sizeOfTemplate );

	BtMemory::Copy( m_pCurrentAddress, &data, sizeOfTemplate );

	m_pCurrentAddress+= sizeOfTemplate;
}

////////////////////////////////////////////////////////////////////////////////
// updateBufferSize

inline void PaMemoryResource::updateBufferSize( BtU32 nLength )
{
	// Grow the memory area if necessary
	BtU32 nCurrentSize = Tell();

	if( ( nCurrentSize + nLength ) > m_nMemorySize )
	{
		m_nMemorySize = nCurrentSize + nLength;		// Create the amount of space we need
		m_nMemorySize += m_nMemorySize / 2;			// And half as much again

		BtU8* pNewMem = BtMemory::Allocate( m_nMemorySize );

		BtAssert( pNewMem != BtNull );

		if( nCurrentSize > 0 )
		{
			BtMemory::Copy( pNewMem, m_pBaseAddress, nCurrentSize );
		}

		BtMemory::Free( m_pBaseAddress );

		// Allocate the new memory. Start using this as the base
		m_pBaseAddress = pNewMem;

		// Set the current pointer to the new memory + the current position
		m_pCurrentAddress = m_pBaseAddress + nCurrentSize;
	}
}

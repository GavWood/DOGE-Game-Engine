////////////////////////////////////////////////////////////////////////////////
// Exporter

// Includes
#include "FsFile.h"
#include "PaMemoryResource.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaMemoryResource::PaMemoryResource( PMR_Usage eUsage, BtU32 maxSize )
{
	m_nMemorySize = maxSize;
	m_pCurrentAddress = m_pBaseAddress = BtMemory::Allocate( m_nMemorySize );
	BtMemory::Set( m_pCurrentAddress, 0, m_nMemorySize );
	m_usage = eUsage;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

PaMemoryResource::~PaMemoryResource()
{
	BtMemory::Free( m_pBaseAddress );
}

////////////////////////////////////////////////////////////////////////////////
// Load

BtBool PaMemoryResource::Load( BtChar* pFilename )
{
	// Ope nthe file
	FsFile file;
	file.Open( pFilename, FsMode_Read );

	if( file.IsOpen() == BtFalse )
	{
		return BtFalse;
	}
	
	// Find the length of the file
	m_nMemorySize = file.GetSize();

	// Free the old memory
	BtMemory::Free( m_pBaseAddress );

	// Create enough memory for the file
	m_pCurrentAddress = m_pBaseAddress = BtMemory::Allocate( m_nMemorySize );

	// Read the file into memory
	file.Read( m_pBaseAddress, m_nMemorySize );

	// Close the file
	file.Close();

	// Return success
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// pCurrent

BtU8* PaMemoryResource::pCurrent()
{
	return m_pCurrentAddress;
}

////////////////////////////////////////////////////////////////////////////////
// pBase

BtU8* PaMemoryResource::GetBase()
{
	return m_pBaseAddress;
}

////////////////////////////////////////////////////////////////////////////////
// Save

void PaMemoryResource::Save( BtChar* pFilename )
{
	// Open the file
	FsFile file;
	file.Open( pFilename, FsMode_Write );
	
	// Read the file into memory
	file.Write( m_pBaseAddress, Tell() );

	// Close the file
	file.Close();
}

////////////////////////////////////////////////////////////////////////////////
// Append

void PaMemoryResource::Append( BtChar* pFilename )
{
	// Open the file in append mode
	FsFile file;
	file.Open( pFilename, FsMode_Append );
	
	// Read the file into memory
	file.Write( m_pBaseAddress, Tell() );

	// Close the file
	file.Close();
}

////////////////////////////////////////////////////////////////////////////////
// GetCurrentSize

BtU32 PaMemoryResource::GetCurrentSize()
{
	return m_nMemorySize;
}

////////////////////////////////////////////////////////////////////////////////
// Tell

BtU32 PaMemoryResource::Tell()
{
	return (BtU32)m_pCurrentAddress - (BtU32)m_pBaseAddress;
}

////////////////////////////////////////////////////////////////////////////////
// operator <<

void PaMemoryResource::operator << ( const PaMemoryItem& item )
{
	BtAssert( m_usage == PMR_Write );

	updateBufferSize( item.m_nSize );

	BtMemory::Copy( m_pCurrentAddress, item.m_pItem, item.m_nSize );

	m_pCurrentAddress+= item.m_nSize;
}

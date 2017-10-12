////////////////////////////////////////////////////////////////////////////////
// SndIff.cpp

// Includes
#include "SndIff.h"
#include <memory.h>
#include <string.h>
#include "BtString.h"

// Make an instance of the singleton
SndIff s_sndIff;

// https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

////////////////////////////////////////////////////////////////////////////////
// Read a sound file

void SndIff::Read( BtU8 *pSource, BtU32 ResourceSize )
{
	m_nStart = pSource;
	m_pCurrent = pSource;

	// Calculate the end of the resource
	m_pEnd = m_nStart + ResourceSize;

	// Process the file
	ProcessChunks();
}

////////////////////////////////////////////////////////////////////////////////
// Skip a number of bytes

void SndIff::SkipBytes( BtU32 nNumBytes )
{
	m_pCurrent += nNumBytes;
}

////////////////////////////////////////////////////////////////////////////////
// Number of bytes to read

BtBool SndIff::ReadBytes( BtU8 *pDest, BtU32 nNumBytes )
{
    // Check if we will over run the end of the file
	if( (m_pCurrent + nNumBytes) >= m_pEnd )
	{
		// Set that we've reached the end of the iff
		m_pCurrent = m_pEnd;

		// Return failure
		return BtFalse;
	}

	// Copy the number of bytes
	memcpy( pDest, m_pCurrent, nNumBytes );

	// Increment the current address
	m_pCurrent += nNumBytes;

	// Return success
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Read the name tag from the IFF chunk

BtSChar *SndIff::ReadTag()
{
	// Read the tag
	if( ReadBytes( (BtU8*) m_tag, 4 ) == BtTrue )
	{
		// Terminate the string
		m_tag[ 4 ] = '\0';
	}
	else
	{
		// Set the string to NULL
		memset( m_tag, 0, sizeof(m_tag) );
	}

	// Return the Tag
	return m_tag;
}

////////////////////////////////////////////////////////////////////////////////
// Read the size from the IFF chunk

BtU32 SndIff::ReadSize()
{
	BtU32 Size;

	// Read the tag
	if( ReadBytes( (BtU8*) &Size, sizeof(BtU32) ) == BtFalse )
	{
		Size = 0;
	}
	return Size;
}

////////////////////////////////////////////////////////////////////////////////
// Process the chunks

void SndIff::ProcessChunks()
{
	BtU32 ChunkSize;

	// Initially flag the data chunk as not found
	BtBool bFoundDataChunk = BtFalse;

	// Process data chunks until we reach the end of the resource
	while( !bFoundDataChunk && ( m_pCurrent < m_pEnd ) )
	{
		// Read the tag from the iff file
		BtSChar *Tag = ReadTag();

		// Compare the tag name
		if( BtStrCompare( Tag, "RIFF") == BtTrue )
		{
			// Size of the RIFF header chunk
			ChunkSize = ReadSize();
		}
		else if( BtStrCompare( Tag, "WAVE" ) == BtTrue )
		{
			BtU32 ForBreakingPointing = 12345;
            (void)ForBreakingPointing;
		}
		else if( BtStrCompare(Tag, "fact" ) == BtTrue )
		{
			// Read the size of the fact chunk
			ChunkSize = ReadSize();
		
			// Skip this number of bytes
			SkipBytes( ChunkSize );
		}
		else if( BtStrCompare(Tag, "data" ) == BtTrue )
		{
			// Flag that we've found the data chunk
			bFoundDataChunk = BtTrue;

			// Read the data size chunk
			m_dataSize = ReadSize();

			// Calculate the data offset
			m_dataOffset = (BtU32) ( m_pCurrent - m_nStart );
		}
		else if( BtStrCompare(Tag, "fmt " ) == BtTrue )
		{		
			// Read the data size chunk
			ChunkSize = ReadSize();

			// Read the format chunk
			ReadBytes( (BtU8*) &m_format, ChunkSize );
		}
	}
}

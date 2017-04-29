////////////////////////////////////////////////////////////////////////////////
// BtCompressedFile.cpp

// Includes
#include "BtCompressedFile.h"
#include "BtBase.h"
#include "BtPrint.h"

////////////////////////////////////////////////////////////////////////////////
// Read

void LBtCompressedFile::Read( FILE *f, BtU8* pMemory, BtU32 targetSize )
{
	BtU32 nPosition = 0;

	// Open the file
	m_file = f;

	// zlib. Allocate inflate state
	m_strm.zalloc   = Z_NULL;
	m_strm.zfree    = Z_NULL;
	m_strm.opaque   = Z_NULL;
	m_strm.avail_in = 0;
	m_strm.next_in  = Z_NULL;
	BtS32 ret = inflateInit( &m_strm );

	if( ret != Z_OK )
	{
		BtPrint( "inflateInit %d.\n", ret );
	}

	BtS32 nRetCode = Z_OK;

	// Read in the data
	do 
	{
		BtU32 nBytesRead = (BtU32)fread( m_buffer, 1, StreamingSize, m_file );

		m_strm.avail_in = nBytesRead;

		if(	m_strm.avail_in == 0 )
		{
			break;
		};

		m_strm.next_in = m_buffer;

		// run inflate() on input until output buffer not full
		do
		{
			// Validate that we've not passed the end of the buffer
			BtAssert( nPosition < targetSize );

			// Calculate the address
			BtU8* pAddress = pMemory + nPosition;

			BtU32 chunkSize = targetSize - nPosition;

			if( chunkSize > StreamingSize )
			{
				chunkSize = StreamingSize;
			}

			m_strm.avail_out = chunkSize;
			m_strm.next_out  = (Bytef*) pAddress;
			nRetCode = inflate( &m_strm, Z_NO_FLUSH );

			// Get how much we are adding
			BtS32 nAmountedInflated = chunkSize - m_strm.avail_out;

			// Increment the position
 			nPosition += (BtU32) nAmountedInflated;

		} while( m_strm.avail_out == 0 );
	
	} while( nRetCode != Z_STREAM_END );

	// zlib. End inflating
	(void)inflateEnd( &m_strm );

	fclose( m_file );
}

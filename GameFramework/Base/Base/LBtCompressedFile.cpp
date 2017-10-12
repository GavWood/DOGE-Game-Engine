////////////////////////////////////////////////////////////////////////////////
// BtCompressedFile.cpp

// Includes
#include "BtCompressedFile.h"
#include "BtBase.h"
#include "BtPrint.h"
#include "ErrorLog.h"

////////////////////////////////////////////////////////////////////////////////
// Read

void BtCompressedFile::Read( FILE *f, BtU8* pMemory, BtU32 targetSize )
{
	// Open the file
	m_file = f;

	// zlib. Allocate inflate state
	m_strm.zalloc   = Z_NULL;
	m_strm.zfree    = Z_NULL;
	m_strm.opaque   = Z_NULL;
	m_strm.avail_in = 0;
	m_strm.next_in  = Z_NULL;
	BtS32 ret = inflateInit( &m_strm );

	BtU8 *pOut = pMemory;

	if( ret != Z_OK )
	{
		BtPrint( "inflateInit %d.\n", ret );
	}

	/* decompress until deflate stream ends or end of file */
	do
	{
		BtU32 nBytesRead = (BtU32)fread(m_buffer, 1, StreamingSize, m_file);

		m_strm.avail_in = nBytesRead;

		if (m_strm.avail_in == 0)
			break;
		m_strm.next_in = m_buffer;

		/* run inflate() on input until output buffer not full */
		do {

			m_strm.avail_out = StreamingSize;
			m_strm.next_out = pOut;
			ret = inflate(&m_strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				
				ErrorLog::Fatal_Printf("inflate %d.\n", ret);

				(void)inflateEnd(&m_strm);
				int a = 0;
				a++;
				return;
			}
			BtU32 have = StreamingSize - m_strm.avail_out;
			
			pOut += have;

		} while (m_strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&m_strm);

	fclose( m_file );
}

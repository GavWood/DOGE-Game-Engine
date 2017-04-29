////////////////////////////////////////////////////////////////////////////////
// BtCompressedFile.h

// Include guard
#pragma once

// Includes
#include "BtTypes.h"
#include "BaFileData.h"
#include "zlib.h"
#include <stdio.h>

const BtU32 StreamingSize = 1024 * 128;

// Class definition
class LBtCompressedFile
{
public:

	// Public functions
	void					Read( FILE *f, BtU8* pMemory, BtU32 targetSize );

private:

	// Private functions
	friend class TdTopEntity;
	friend class TdDebug;

	FILE*					m_file;
	z_stream				m_strm;
	BtU8					m_buffer[StreamingSize];
};

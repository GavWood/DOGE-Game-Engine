////////////////////////////////////////////////////////////////////////////////
// SndIff.h

// Include guard
#pragma once
#include "BtTypes.h"

////////////////////////////////////////////////////////////////////////////////
// SndFMT format "fmt " structure

struct SndWavFormat
{
		BtU16							m_formatTag;				// 2	Format tags indicating ADPCM
		BtU16							m_channels;					// 2	1 mono, 2 stereo
		BtU32							m_sampleRate;				// 4	44100, 22050, etc...
		BtU32							m_averageBytesPerSec;		// 4
		BtU16							m_blockAlign;				// 2
		BtU16							m_bitsPerSample;			// 2	8 bit or 16 bit file
		BtU16							m_extraParams;				// 2	
};

// Class definition
class SndIff
{
	public:

		// Public functions
		void							Read( BtU8 *pSource, BtU32 ResourceSize );

		// Accessors					
		SndWavFormat&					GetWavFormat();
		BtU32							GetDataOffset();
		BtU32							GetDataSize();
		BtU8*							GetRawData();

	private:

		// Private functions
		void							ProcessChunks();

		BtBool							ReadBytes( BtU8 *pDest, BtU32 nNumBytes );

		BtSChar*						ReadTag();

		void							SkipBytes( BtU32 nNumBytes );

		BtU32							ReadSize();

		// Private members
		SndWavFormat					m_format;

		BtU32							m_dataOffset;
		
		BtU8*							m_nStart;
		
		BtU8*							m_pCurrent;
		
		BtU8*							m_pEnd;
		
		BtU32							m_dataSize;

		BtSChar							m_tag[5];
};

////////////////////////////////////////////////////////////////////////////////
// GetRawData

inline BtU8* SndIff::GetRawData()
{
	return m_nStart + m_dataOffset;
}

////////////////////////////////////////////////////////////////////////////////
// GetDataSize

inline BtU32 SndIff::GetDataSize()
{
	return m_dataSize;
}

////////////////////////////////////////////////////////////////////////////////
// GetDataOffset

inline BtU32 SndIff::GetDataOffset()
{
	return m_dataOffset;
}

////////////////////////////////////////////////////////////////////////////////
// GetWavFormat

inline SndWavFormat& SndIff::GetWavFormat()
{
	return m_format;
}

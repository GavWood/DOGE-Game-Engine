////////////////////////////////////////////////////////////////////////////////
// ExSound

#ifndef __ExSound_h__
#define __ExSound_h__

#include "BtTypes.h"
#include "PaPacker.h"
#include "ExResource.h"

class ExSound : public ExResource
{
public:

	void							Export();

private:

	void							LoadSampleFile();

	// Members
	BtU8*							m_pRawFileData;
	BtU32							m_nFileSize;
};

#endif // __ExSound_h__

////////////////////////////////////////////////////////////////////////////////
// PaPackerInterface

#ifndef __PaPackerInterface_h__
#define __PaPackerInterface_h__

#include "BtTypes.h"
#include "PaFileDetails.h"
#include "PaMemoryResource.h"

class PaPackerInterface
{
public:

	PaPackerInterface() : m_memoryResource( PMR_Write )
	{

	}

	// The file we are exporting
	BtBool					 m_bExport;

	const PaFileDetails*	 pFile() const;
	const BtChar*			 pWorkingPath() const;
	const void*				 pDevice() const;
	PaMemoryResource&		 MemoryResource();

private:

	friend class ExExporter;

	PaFileDetails*			 m_pFile;
	BtChar					 m_szWorkingPath[256];
	void*					 m_pDevice;
	PaMemoryResource		 m_memoryResource;
};

inline const PaFileDetails* PaPackerInterface::pFile() const
{
	return m_pFile;
}

inline const BtChar* PaPackerInterface::pWorkingPath() const
{
	return m_szWorkingPath;
}

inline const void* PaPackerInterface::pDevice() const
{
	return m_pDevice;
}

inline PaMemoryResource& PaPackerInterface::MemoryResource()
{
	return m_memoryResource;
}

#endif // __PaPacker_h__

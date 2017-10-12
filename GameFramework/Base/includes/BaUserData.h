////////////////////////////////////////////////////////////////////////////////
/// BaUserData.h

#ifndef __BaUserData_h__
#define __BaUserData_h__

#include "BtTypes.h"
#include "BaResource.h"

class BaArchive;

// Class Declaration
class BaUserData : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_UserData );

	void							FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	BtU8*							GetData();
	BtU32							GetSize();

private:

	friend class BaArchiveManager;

	BtU32							m_nSize;
	BtU8*							m_pFileData;
	BtU32							m_nAlign;
};

#endif // __LBaUserData_h__

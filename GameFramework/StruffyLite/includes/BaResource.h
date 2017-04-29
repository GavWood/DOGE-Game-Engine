////////////////////////////////////////////////////////////////////////////////
/// BaResource.h

// Include guard
#pragma once
#include "BtPrint.h"
#include "BtTypes.h"
#include "BaArchive.h"

#define DECLARE_RESOURCETYPE( _TyID ) \
	BaResourceType resourceType() const \
		{ \
		return _TyID; \
	} \

// Class Declaration
class BaResource
{
public:

	BaResource();

	virtual BaResourceType resourceType() const
	{
		return BaRT_Max;
	}

	virtual void 					FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	virtual void 					Remove();
	virtual void 					Destroy();
	virtual void 					CreateOnDevice();
	virtual void 					RemoveFromDevice();

	// Accessors
	void							SetTitle( BtChar* pTitle );
	BtChar*							GetTitle() const;
	BtChar*							GetArchiveTitle() { return m_pArchive->GetFilename(); }
	BtBool							IsDuplicate() const;

protected:

	friend class BaArchive;

	BtChar*							m_pTitle;
	BtBool							m_isDuplicate;
	BaArchive*						m_pArchive;
};

////////////////////////////////////////////////////////////////////////////////
// PaFilename.h

// Include guard
#ifndef __PaFilename_h__
#define __PaFilename_h__

#include "BtTypes.h"
#include "BtString.h"
#include "BaFileData.h"

class PaFilename
{
public:
	
	PaFilename();
	PaFilename( const BtChar* pFilename );

	const BtChar*	GetFilename() const;

private:

	BtChar			m_szFilename[MaxFilePath];
};

inline PaFilename::PaFilename()
{
}

inline PaFilename::PaFilename( const BtChar* pFilename )
{
	BtStrCopy( m_szFilename, MaxFilePath, pFilename );
}

inline const BtChar* PaFilename::GetFilename() const
{
	return m_szFilename;
}

#endif // __PaFilename_h__

////////////////////////////////////////////////////////////////////////////////
// UtFilename.h

// Include guard
#ifndef __UtFilename_h__
#define __UtFilename_h__

#include "BtTypes.h"

class UtFilename
{
public:

	// Constructor
	UtFilename( BtChar* filename );

	// Accessors
	const BtChar*			GetExtension() const;
	const BtChar*			GetPathname() const;
	const BtChar*			GetFilename() const;
	const BtChar*			GetTitle() const;

private:

	BtChar					m_pathname[256];
	BtChar					m_extension[256];
	BtChar					m_filename[256];
	BtChar					m_title[256];
};

////////////////////////////////////////////////////////////////////////////////
// GetExtension

inline const BtChar* UtFilename::GetExtension() const
{
	return m_extension;
}

////////////////////////////////////////////////////////////////////////////////
// GetPathname

inline const BtChar* UtFilename::GetPathname() const
{
	return m_pathname;
}

////////////////////////////////////////////////////////////////////////////////
// GetTitle

inline const BtChar* UtFilename::GetTitle() const
{
	return m_title;
}

////////////////////////////////////////////////////////////////////////////////
// GetFilename

inline const BtChar* UtFilename::GetFilename() const
{
	return m_filename;
}

#endif // __UtFilename_h__

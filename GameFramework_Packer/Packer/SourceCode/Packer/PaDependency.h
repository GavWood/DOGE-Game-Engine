////////////////////////////////////////////////////////////////////////////////
// PaDependency

#ifndef __PaDependency_h__
#define __PaDependency_h__

#include "PaFileDetails.h"

class PaDependency
{
public:

	// Constructor
	PaDependency( const PaFileDetails& file )
	{
		m_file = file;
	}

	const PaFileDetails&			File() const;

private:

	PaFileDetails					m_file;								// File details
};

////////////////////////////////////////////////////////////////////////////////
// File

inline const PaFileDetails& PaDependency::File() const
{
	return m_file;
}

#endif // __PaDependency_h__

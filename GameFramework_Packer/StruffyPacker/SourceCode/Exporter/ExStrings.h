////////////////////////////////////////////////////////////////////////////////
// ExStrings

#ifndef __ExStrings_h__
#define __ExStrings_h__

#include <vector>
#include <string>
#include "BtTypes.h"
#include "TeTexture.h"
#include "PaPacker.h"
#include "ExResource.h"

class ExStrings : public ExResource
{
public:

	void							Export();

private:

    std::vector<std::string>		m_strings;
};

#endif // __ExStrings_h__

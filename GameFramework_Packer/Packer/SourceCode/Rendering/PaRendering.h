////////////////////////////////////////////////////////////////////////////////
// PaExportSizes.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "ExMaterialBlock.h"

// Class definition
class PaExportSizes
{
public:

	// Public functions
	void							Run();

	// Platform specific types where they can't be avoided
	static BtU32					GetTriangleType(ExPrimitiveType primitiveType);

private:

	// Private functions

	// Private members

};

////////////////////////////////////////////////////////////////////////////////
// ExFont

#pragma once
#include "BtTypes.h"
#include "PaPacker.h"
#include "PaFileDetails.h"
#include "ExResource.h"

class ExFont : public ExResource
{
public:

	void							 Export();

private:

	void							 ExportAngelCodeFont();
	void							 ExportTTFFont();
};

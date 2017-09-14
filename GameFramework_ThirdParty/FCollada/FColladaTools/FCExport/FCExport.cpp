/*
    Copyright (C) 2005-2007 Feeling Software Inc.
    MIT License: http://www.opensource.org/licenses/mit-license.php

    Portions of the code surrounded by PREMIUM conditional compilation
    blocks are copyright Feeling Software Inc., strictly confidential and released
    to Premium Support licensees only. Licensees are allowed to modify and extend
    the Premium code for their internal use. Redistributing the Premium source code
    or any of its derivative product (e.g. binary versions) is forbidden.
*/

/*
	FCExport opens a COLLADA document and saves it. Format is defined by the file
    extension of the output file.
*/

#include "StdAfx.h"
#include "FCDocument.h"

int main(int argc, const char* argv[], char* envp[])
{
#ifdef WIN32
	_environ = envp;
#else //LINUX
	environ = envp;
#endif //WIN32 and LINUX
	if (argc != 3)
	{
		std::cout << "Expecting two arguments:" << std::endl;
		std::cout << "FCExport.exe <input_filename> <output_filename>" <<std::endl;
		exit(-1);
	}

	fstring inputFilename = TO_FSTRING(argv[1]);
	fstring outputFilename = TO_FSTRING(argv[2]);

	// Create an empty COLLADA document and import the given file.
	FCollada::Initialize();
	FCDocument* document = FCollada::NewTopDocument();
	FUErrorSimpleHandler errorHandler;
	FCollada::LoadDocumentFromFile(document, inputFilename.c_str());
	if (errorHandler.IsSuccessful())
	{
		// Write out the COLLADA document.
		std::cout << "Exporting: ";
		FCollada::SaveDocument(document, outputFilename.c_str());
		if (errorHandler.IsSuccessful())
		{
			std::cout << "Done." << std::endl;
		}
		else
		{
			std::cout << errorHandler.GetErrorString();
			std::cout << std::endl << std::endl;
		}
	}
	else
	{
		std::cout << errorHandler.GetErrorString();
		std::cout << std::endl << std::endl;
	}

	SAFE_DELETE(document);
	FCollada::Release();
	return 0;
}

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
	FCValidate is a small tool to validate a given COLLADA file against FCollada.
	TODO: Add schema validation.
*/

#include "StdAfx.h"
#include "FCDocument.h"

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Expecting at least one argument: the filename(s) to validate." << std::endl;
		exit(-1);
	}

	FCollada::Initialize();

	--argc; ++argv;
	for (; argc > 0; --argc, ++argv)
	{
		FUErrorSimpleHandler errorHandler;
		FCDocument* document = FCollada::NewTopDocument();
		fstring filename = TO_FSTRING(argv[0]);
		FCollada::LoadDocumentFromFile(document, filename.c_str());

		std::cout << argv[0] << std::endl;
		std::cout << errorHandler.GetErrorString();
		std::cout << std::endl << std::endl;
		SAFE_DELETE(document);
	}

	FCollada::Release();

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// FsFindFile.h

// Include guard
#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>
#include "PaFileDetails.h"
#include "PaFilename.h"
#include "BtTypes.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

// A list of found files
typedef vector<PaFileDetails> FileList_t;

class FsFindFile
{
	// Scans a path for files as according to 
	void                                scanPath(string path);
    
public:
    
	 FsFindFile( string location );

	// Finds all files
	void                                search();

	string m_location;
    
    FileList_t                          filelist;        // List of files found in search
    BtU64                               listsize;        // Size in bytes of all files in found list
};

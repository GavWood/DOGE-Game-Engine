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

using namespace std;

// A list of found files
typedef vector<PaFileDetails> FileList_t;

class FsFindFile
{
private:

	// Scans a path for files as according to 
	void scanPath(string path);

#ifdef WIN32
	// Finds a single file and returns true if there are more to come
	bool getFiles(HANDLE &searchHandle, WIN32_FIND_DATA &filedata, string path);
#else
    bool getFiles();
#endif
    
public:
    
	 FsFindFile( string location );
	~FsFindFile();

	// Clears list of found files, file handle and so on
	void clear();

	// Finds all files as specified in the initial options
	void search ();

	// Concatenates 2 paths
	static string combinePath(string path1, string path2);

	string m_location;
    
    FileList_t                          filelist;        // List of files found in search
    BtU64                               listsize;        // Size in bytes of all files in found list

};
#endif

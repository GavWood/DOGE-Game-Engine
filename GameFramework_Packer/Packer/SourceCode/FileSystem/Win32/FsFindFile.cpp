#include "FsFindFile.h"

#ifdef WIN32

FsFindFile::FsFindFile( string location )
{
    m_location = location;
}
//--------------------------------------------------------------------------

FsFindFile::~FsFindFile()
{
    clear();
}
//--------------------------------------------------------------------------

// Combines the two path's together and returns the combined path. This function
// eliminates the problem of combining directories when the left path may or
// may not contain a backslash. eg "c:\ and windows" or "c:\windows and system"
string FsFindFile::combinePath (string path1, string path2)
{
    if (path1.find_last_of("\\") != path1.length()-1 && path2 != "")
	{
        path1 += "\\";
	}

    path1 += path2;

    return path1;
}
//------------------------------------------------------------------------------

// Re-initializes the FileFind object so that it can be reused without freeing
// and allocating memory every time
void FsFindFile::clear ()
{
    filelist.clear ();
    listsize = 0;
}
//---------------------------------------------------------------------------


// Searches the location directory for all files and returns
// true if more files may be available and false if that was the last one
bool FsFindFile::getFiles( HANDLE& searchHandle, WIN32_FIND_DATA& fileData, string path )
{
    BtU32 nValid;
    
    if (searchHandle == NULL)
    {
        string pathToSearch = combinePath(path, "*");
        
        searchHandle = FindFirstFile(pathToSearch.c_str(), &fileData);
        nValid = (searchHandle == INVALID_HANDLE_VALUE) ? 0 : 1;
    }
    else
    {
        nValid = FindNextFile(searchHandle, &fileData);
    }
    
    while (nValid)
    {
        // As long as this file is not . or .., we are done
        if( BtStrCompare( fileData.cFileName, ".") == BtFalse &&
           BtStrCompare( fileData.cFileName, "..") == BtFalse )
            return BtTrue;
        
        nValid = FindNextFile(searchHandle, &fileData);
    }
    
    FindClose(searchHandle);
    searchHandle = NULL;
    
    return BtFalse;
}

//---------------------------------------------------------------------------
// Finds all files as specified in the initial options

void FsFindFile::search ()
{
    clear();
    scanPath( m_location );
}
//---------------------------------------------------------------------------


// Scans a path for files as specified in the filter and stores them in the
// file list array. If a recursive options was specified, scanPath will
// continue to search for files in all subdirectories.
void FsFindFile::scanPath(string path)
{
    WIN32_FIND_DATA fileData;
    PaFileDetails fi;

    HANDLE searchHandle = NULL;

    while( getFiles( searchHandle, fileData, path ) )
    {
        // Skip hidden files
		if( fileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
		{
			continue;
		}

		// Set most of the file attributes from the filename
		PaFileDetails fi = PaFileDetails( &path[0], &fileData.cFileName[0] );

		// Set the is folder flag
		if( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			fi.m_bFolder = BtTrue;
		}
		else
		{
			fi.m_bFolder = BtFalse;
		}

		memcpy( &fi.m_tResourceChanged, &fileData.ftLastWriteTime, sizeof(BtU64) );

		// Add the file
		filelist.push_back (fi);
		listsize += fileData.nFileSizeLow + fileData.nFileSizeHigh * MAXDWORD;

		// Recurse
		if( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			scanPath( combinePath ( path, fileData.cFileName) );
		}
    }
}

#endif

//---------------------------------------------------------------------------

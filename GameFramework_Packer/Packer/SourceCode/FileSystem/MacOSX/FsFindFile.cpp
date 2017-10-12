#include "FsFindFile.h"
#include "FsFile.h"
#include "ErrorLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

FsFindFile::FsFindFile( string location )
{
    m_location = location;
}

//---------------------------------------------------------------------------
// Finds all files as specified in the initial options

void FsFindFile::search ()
{
    listsize = 0;
    filelist.clear();
    scanPath( m_location );
}
//---------------------------------------------------------------------------


// Scans a path for files as specified in the filter and stores them in the
// file list array. If a recursive options was specified, scanPath will
// continue to search for files in all subdirectories.
void FsFindFile::scanPath(string path)
{
    char FS = FsFile::GetFileSeparator();
    DIR* FD;
    struct dirent* in_file;
    
    const char *in_path = (const char*)path.c_str();
    if (NULL == (FD = opendir ( in_path )))
    {
        fprintf(stderr, "Error : Failed to open input directory\n");
        return;
    }
    
    while ((in_file = readdir(FD)))
    {
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))
            continue;

        // Get the full filename
        BtChar fileSeparator[4];
        sprintf(fileSeparator, "%c", FsFile::GetFileSeparator() );
        string fileName = path + string(fileSeparator) + string( in_file->d_name );

        // for debugging
        ErrorLog::Printf( "in_file->d_name %s", in_file->d_name );
        //ErrorLog::Printf( "in_path %s", in_path );
        
        // Set from the path and filename
        PaFileDetails fileDetails;
        
        strcpy( fileDetails.m_szFilename, (const char*)fileName.c_str() );
        strcpy( fileDetails.m_szPath, in_path) ;
        strcpy( fileDetails.m_szName, in_file->d_name ) ;
        
        // Get the extension
        BtChar *pExtension = strrchr( fileDetails.m_szFilename, '.' );
        if( pExtension )
        {
            strcpy( fileDetails.m_szExtension, pExtension + 1 );
        }
        else
        {
            strcpy( fileDetails.m_szExtension, "" );
        }
        
        // Construct the folder name
        {
            BtChar *szFolder = strrchr( fileDetails.m_szPath, FS );
            
            if( szFolder )
            {
                sprintf( fileDetails.m_szFolder, "%s", szFolder + 1 );
            }
            else
            {
                strcpy( fileDetails.m_szFolder, fileDetails.m_szPath );
            }
        }
        
        // Get the title
        {
            strcpy( fileDetails.m_szTitle, in_file->d_name );
            
            // Find the extension
            pExtension = strrchr( fileDetails.m_szTitle, '.' );
            
            // Copy the extension
            if( pExtension )
            {
                *pExtension = 0;
            }
        }
        
        // Set the extension to lower case
        BtStrLower( fileDetails.m_szExtension, MaxFilePath );
        BtStrLower( fileDetails.m_szName, MaxFilePath );
        BtStrLower( fileDetails.m_szTitle, MaxFilePath );
        BtStrLower( fileDetails.m_szFilename, MaxFilePath );
        BtStrLower( fileDetails.m_szPath, MaxFilePath );
        BtStrLower( fileDetails.m_szFolder, MaxFilePath );
        
        BtBool isDebug = 1;// BtFalse;
        if( isDebug )
        {
            ErrorLog::Printf( "Name: %s", fileDetails.m_szName );
            ErrorLog::Printf( "Extension: %s", fileDetails.m_szExtension );
            ErrorLog::Printf( "Path: %s", fileDetails.m_szPath );
            ErrorLog::Printf( "Filename: %s", fileDetails.m_szFilename );
            ErrorLog::Printf( "Title: %s", fileDetails.m_szTitle );
            ErrorLog::Printf( "Folder: %s", fileDetails.m_szFolder );
            ErrorLog::Printf( "\r\n" );        //ErrorLog::Printf( "Title %s",    fileDetails.m_szTitle );
        }
        
        if( in_file->d_type == 4 )
        {
            // Mark this as a folder
            fileDetails.m_bFolder = BtTrue;
            
            // Add this file to the array
            filelist.push_back(fileDetails);
            
            scanPath(fileName);
        }
        else
        {
            // Get the file stat
            struct stat attrib;
            stat( fileDetails.m_szFilename, &attrib);
            
            // Get the time of last modification
            time_t lastChanged = attrib.st_mtime;
    
            // Set the time the file was last changed
            fileDetails.m_tResourceChanged = lastChanged;
            
            // Mark this as a file
            fileDetails.m_bFolder = BtFalse;
            
            // Add this file to the array
            filelist.push_back(fileDetails);
            
            // ErrorLog::Printf( "Filename %s", filename );
        }
        
        int a=0;
        a++;
    }
}

//---------------------------------------------------------------------------

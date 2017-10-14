////////////////////////////////////////////////////////////////////////////////
// ExStrings

// Includes
#include "BtTypes.h"
#include "BtMemory.h"
#include "BaArchive.h"
#include "ExStrings.h"
#include "BaArchive.h"
#include "BtString.h"
#include "ErrorLog.h"
#include "PaTopState.h"
#include "FsFile.h"

class StStrings
{
	int todo;
};

////////////////////////////////////////////////////////////////////////////////
// Export

void ExStrings::Export()
{
    char FS = FsFile::GetFileSeparator();
	BtChar id[256];
	BtChar contents[256];
	BtChar szFilename[256];

	PaMemoryResource strings(PMR_Write);

	// Open the string file
	FILE *fStrings = fopen( GetFilename(), "rb" );

	// Get the top state
	PaTopState* pTopGameState = PaTopState::InstancePtr();

	// Open the id file
	sprintf( szFilename, "%s%cSourceCode%c%s.h", pTopGameState->pWorkingPath(), FS, FS, GetTitle() );

	FILE *fIDs = fopen( szFilename, "w+" );

	if( fIDs == NULL )
	{
		ErrorLog::Fatal_Printf( "Could not open string resource file.\n" );
	}
	
	// Export the ids into the source code folder
	fprintf(fIDs, "////////////////////////////////////////////////////////////////////////////////\n");
	fprintf(fIDs, "// %s.h\n\n", GetTitle() );
	fprintf(fIDs, "// Include guard\n" );
	fprintf(fIDs, "#ifndef __%s_h__\n", GetTitle() );
	fprintf(fIDs, "#define __%s_h__\n\n", GetTitle() );

	// Start at a zero index for string ids
	BtU32 index = 0;

	// Clear the vector of strings
	m_strings.clear();

	// While the are lines remaining
	while( !feof(fStrings) )
	{
		// Read in the id
		if( fscanf( fStrings, "%s\n", (char*)&id ) == -1 )
		{
			break;
		}

		// Write out the ids
		fprintf( fIDs, "#define %s %d\n", id, index );

		// Read in the line
		fgets( contents, 100, fStrings );

		// Add the string
		m_strings.push_back( contents );

		// Increment the index
		index++;
	}

	// End the file
	fprintf(fIDs, "\n#endif // __%s_h__\n", GetTitle() );

	// Close the string file
	fclose( fStrings );

	// Close the ids file
	fclose( fIDs );


	// Cache the number of strings
	BtU32 nNumStrings = (BtU32) m_strings.size();

	// Calculate the size of the lookup table
	BtU32 nLookupTableSize = nNumStrings * sizeof( BtU32 );

	BaStringFileData fileData;

	// Calculate the running offset
	BtU32 nOffset = sizeof( BaStringFileData ) + nLookupTableSize;

	// Copy the offsets into the memory
	for( BtU32 i=0; i<nNumStrings; i++ )
	{
		// Add an integer
		strings << nOffset;

		// Cache each string
		BtChar* pString = (BtChar*) m_strings[i].c_str();

		// Add the string offset
		nOffset += (BtU32) ( strlen( pString ) ) + 1;
	}

	// Set the number of strings
	fileData.nNumStrings = nNumStrings;
	
	strings << fileData;

	// Copy the strings across
	for( BtU32 i=0; i<nNumStrings; i++ )
	{
		// Cache each string
		BtChar* pString = (BtChar*) m_strings[i].c_str();

		// Copy the strings
		strings << PaMemoryItem( (BtU8*) pString, (BtU32) strlen( pString ) );
	}

	AddResource( strings, GetFilename(), GetTitle(), BaRT_Strings );
}

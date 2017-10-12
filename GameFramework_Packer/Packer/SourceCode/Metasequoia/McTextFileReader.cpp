////////////////////////////////////////////////////////////////////////////////
// MCTextFileReader.cpp

// Includes
#include "BtArray.h"
#include "McTextFileReader.h"
#include "BtMemory.h"
#include "stdio.h"
#include "stdlib.h"
#include <string>

////////////////////////////////////////////////////////////////////////////////
// IsNewLine

BtBool IsNewLine( BtChar* pCharacter )
{
	BtChar character = *pCharacter;

	switch( character )
	{
	case 13:
		return BtTrue;

	case 10:
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsNumber

BtBool IsNumber( BtChar character )
{
	switch( character )
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsQuotes

BtBool IsQuotes( BtChar character )
{
	switch( character )
	{
	case '\"':
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsSeparator

BtBool IsSeparator( BtChar character )
{
	switch( character )
	{
	case '\t':
		return BtTrue;

	case 10:
		return BtTrue;

	case 13:
		return BtTrue;

	case ' ':
		return BtTrue;

	case ',':
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

MCTextFileReader::MCTextFileReader()
{
}

////////////////////////////////////////////////////////////////////////////////
// AddBracktedToken

void MCTextFileReader::AddBracktedToken( const BtChar* pToken )
{
	BtString newString;

	BtStrCopy( newString, 256, pToken );

	// Remove any remaining right bracket
	BtChar* rightBracket = strrchr( newString, ')' );
	
	if( rightBracket )
	{
		*rightBracket = '\0';
	}
    
    std::string myString( newString );

	// Add this value
	m_values.Add( myString );
}

////////////////////////////////////////////////////////////////////////////////
// GetBracktedExpression

BtS32 MCTextFileReader::GetBracktedExpression( BtU32 iLine, const BtChar* pToken )
{
	// Empty the values
	m_values.Empty();

	// Find the string length
	BtU32 stringLength = BtStrLength( pToken );

	// Get the number of tokens
	BtU32 numTokens = GetNumTokens( iLine );

	// Loop through the tokens
	for( BtU32 iToken=0; iToken<numTokens; iToken++ )
	{
		const BtChar* pFoundToken = GetToken( iLine, iToken );

		const BtChar* pEachToken = pFoundToken;

		if( BtStrCompare( pFoundToken, pToken, stringLength ) == BtTrue )
		{
			// Start off after the "token("
			pEachToken = pFoundToken + stringLength;

			// Add the brackted token
			AddBracktedToken( pEachToken );

			if( BtStrStr( pEachToken, ")" ) == 0 )
			{
				// Loop through the subsequent tokens
				for( BtU32 iToken2=iToken + 1; iToken2<numTokens; iToken2++ )
				{
					pEachToken = GetToken( iLine, iToken2 );

					// Add the brackted token
					AddBracktedToken( pEachToken );

					if( BtStrStr( pEachToken, ")" ) )
					{
						break;
					}
				}
			}

			// Cache the number of items
			BtU32 items = m_values.GetNumItems();

			// Return this value
			return items;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// GetBracktedExpressionValue

BtFloat MCTextFileReader::GetBracktedExpressionAsFloat( BtU32 iIndex )
{
	const BtChar* token = GetBracktedExpressionAsString( iIndex );
	BtFloat floatValue = (BtFloat)atof( token );
	return floatValue;
}

////////////////////////////////////////////////////////////////////////////////
// GetBracktedExpressionAsString

const BtChar* MCTextFileReader::GetBracktedExpressionAsString( BtU32 iIndex )
{
	return m_values[iIndex].c_str();
}

////////////////////////////////////////////////////////////////////////////////
// Open

void MCTextFileReader::Open( const BtChar* filename )
{
	FILE* f;

	// Open the file
	f = fopen( filename, "rb"	);

	if( f != BtNull )
	{
		// Seek the end of the file
		fseek( f, 0, SEEK_END );

		// Get the position
		BtU32 nSize = ftell( f );

		// Seek the start of the file
		fseek( f, 0, SEEK_SET );

		// Allocate the memory for the dictionary file
		BtU8* pData = BtMemory::Allocate( nSize );

		// Read in the whole file
		fread( pData, nSize, 1, f );

		// Read the file from one single memory chunk
		Open( pData, nSize );

		// Deallocate the memory
		BtMemory::Free( pData );

		fclose( f );
	}
	else
	{
		ErrorLog::Fatal_Printf( "Could not open file %s\r\n", filename );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Open

void MCTextFileReader::Open( BtU8* pData, BtU32 dataSize )
{
	m_strings.Empty();
	m_textLines.Empty();

	BtChar* pStringStart;

	// Start reading the lines
	BtChar* pStart  = (BtChar*)pData;
	BtChar* pCurrent = pStart;

	BtChar* pEnd =  pStart + dataSize;

	MCTextLine textLine;
	textLine.m_first = 0;

	BtBool quotes = BtFalse;

	while( pCurrent < pEnd )
	{
		BtBool newLine = BtFalse;

		while( pCurrent < pEnd )
		{
			if( IsSeparator( *pCurrent ) == BtFalse )
			{
				break;
			}

			if( IsNewLine( pCurrent ) == BtTrue )
			{
				newLine = BtTrue;
			}
			++pCurrent;
		}

		if( pCurrent >= pEnd )
		{
			break;
		}

		if( BtStrCompare( (char*)pCurrent, "tex(" , 4 ) == BtTrue )
		{
			int a=0;
			a++;
		}

		// Start outside some quotes
		quotes = BtFalse;

		// Skip past the string
		pStringStart = pCurrent;

		while( pCurrent < pEnd )
		{
			if( IsQuotes( *pCurrent ) == BtTrue )
			{
				quotes = !quotes;
			}

			if( !quotes )
			{
				if( IsSeparator( *pCurrent ) == BtTrue )
				{
					break;
				}
			}

			++pCurrent;
		}

		if( newLine == BtTrue )
		{
			textLine.m_last = m_strings.GetNumItems();
			m_textLines.Add( textLine );

			textLine.m_first = m_strings.GetNumItems();
		}

		// Get the length
		BtU32 length = pCurrent - pStringStart;

		// Copy the label
		BtString sourceString, destString;
		BtMemory::Set( sourceString, 0, sizeof( sourceString ) );
		BtMemory::Copy( sourceString, pStringStart, length );

		// Remove quotes
		BtStringReplace( sourceString, destString, 256, "\"", "" );

		// Make a string
		std::string temp( destString );

		// Add the new string
		m_strings.Add( temp );
	}
	
	// Add the last one
	textLine.m_last = m_strings.GetNumItems();
	m_textLines.Add( textLine );
}

////////////////////////////////////////////////////////////////////////////////
// GetFullLine

const BtChar* MCTextFileReader::GetFullLine( BtU32 line )
{
	BtStrEmpty( m_fullLine, 256 );

	for( BtU32 iString=m_textLines[line].m_first; iString<m_textLines[line].m_last; iString ++ )
	{
		BtStrCat( m_fullLine, 256, m_strings[iString].c_str() );
		BtStrCat( m_fullLine, 256, " " );
	}
	return m_fullLine;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumTokens

BtU32 MCTextFileReader::GetNumTokens( BtU32 line ) const
{
	return m_textLines[line].m_last - m_textLines[line].m_first;
}

////////////////////////////////////////////////////////////////////////////////
// GetToken

const BtChar* MCTextFileReader::GetToken( BtU32 line, BtU32 token ) const
{
	BtU32 index = m_textLines[line].m_first + token;
	return m_strings[index].c_str();
}

////////////////////////////////////////////////////////////////////////////////
// GetTokenAsString

const BtChar* MCTextFileReader::GetTokenAsString( BtU32 line, BtU32 token )
{
	return GetToken( line, token );
}

////////////////////////////////////////////////////////////////////////////////
// GetTokenAsInteger

BtS32 MCTextFileReader::GetTokenAsInteger( BtU32 line, BtU32 token ) const
{
	const BtChar* pValue = GetToken( line, token );

	BtS32 numValue = atoi( pValue );

	return numValue;
}

////////////////////////////////////////////////////////////////////////////////
// GetTokenAsFloat

BtFloat MCTextFileReader::GetTokenAsFloat( BtU32 line, BtU32 token ) const
{
	const BtChar* pValue = GetToken( line, token );
	BtFloat numValue = (BtFloat)atof( pValue );

	return numValue;
}

////////////////////////////////////////////////////////////////////////////////
// UtTokeniser.cpp

#include "BtString.h"
#include "UtTokeniser.h"
#include "stdlib.h"
#include "ErrorLog.h"

///////////////////////////////////////////////////////////////////////////////
// UtTokeniser

UtTokeniser::UtTokeniser()
{
	m_file = BtNull;
	m_numTokens = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Read

BtBool UtTokeniser::Read( const BtChar* pName )
{
	// Open the material file
	m_file = fopen( pName, "r" );

	if( m_file == BtNull )
	{
		return BtFalse;
	}

	BtChar line[MaxTokenSize];

	m_numTokens = 0;

	while( !feof(m_file) )
	{
		// Read in the token
		if( fgets( line, MaxTokenSize, m_file ) == NULL )
		{
			break;
		}

		BtStrLower( line, MaxTokenSize );

		BtU32 length = BtStrLength( line );
		BtBool valid = BtFalse;

		for( BtU32 i=0; i<length; i++ )
		{
			BtChar str[2];
			str[0] = line[i];
			str[1] = '\0';

			if( BtStrStr( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", str ) )
			{
				valid = BtTrue;
				break;
			}
		}

		if( valid == BtFalse )
		{
			ErrorLog::Printf( "%s Don't place blank lines\r\n", pName );
			return BtFalse;
		}

		BtChar* pValue = BtStrStr( line, " " );

		if( pValue == BtNull )
		{
			ErrorLog::Printf( "Line in %s must have a token\r\n", pName );
			return BtFalse;
		}

		UtToken token;

		// Copy the value
		BtStrCopy( token.m_value, MaxTokenSize, pValue + 1, MaxTokenSize );

		// Trim it
		BtStrTrimNewLines( token.m_value, 256 );

		// Copy the name
		*pValue = '\0';
		BtStrCopy( token.m_name, MaxTokenSize, line, MaxTokenSize );

		token.m_bool = BtFalse;

		if( BtStrCompare( token.m_value, "true", 3 ) == BtTrue )
		{
			token.m_bool = BtTrue;
		}
		if( BtStrCompare( token.m_value, "yes", 3 ) == BtTrue )
		{
			token.m_bool = BtTrue;
		}
		else if( BtStrCompare( token.m_value, "on", 2 ) == BtTrue )
		{
			token.m_bool = BtTrue;
		}
		else if( BtStrCompare( token.m_value, "1", 1 ) == BtTrue )
		{
			token.m_bool = BtTrue;
		}

		// Set the token
		m_tokens[m_numTokens] = token;

		// Go to the next token
		++m_numTokens;
	}

	fclose( m_file );
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// GetToken

BtBool UtTokeniser::GetToken( const BtChar* pName )
{
	BtChar compare[256];

	// Copy the string
	BtStrCopy( compare, 256, pName );

	// Make it lower case
	BtStrLower( compare, 256 );

	for( BtU32 iToken=0; iToken<m_numTokens; iToken++ )
	{
		m_current = m_tokens[iToken];

		if( BtStrCompare( m_current.m_name, pName ) == BtTrue )
		{
			return BtTrue;
		}
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetBool

BtBool UtTokeniser::GetBool( const BtChar* pName, BtBool& value )
{
	if( GetToken( pName ) == BtTrue )
	{
		value = m_current.m_bool;
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNum

BtBool UtTokeniser::GetNum( const BtChar* pName, BtU32& value )
{
	if( GetToken( pName ) == BtTrue )
	{
		value = atoi(  m_current.m_value );
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetFloat

BtBool UtTokeniser::GetFloat( const BtChar* pName, BtFloat& value )
{
	if( GetToken( pName ) == BtTrue )
	{
		value = (BtFloat)atof( m_current.m_value );
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetString

BtBool UtTokeniser::GetString( const BtChar* pName, BtChar* pValue, BtU32 size )
{
	if( GetToken( pName ) == BtTrue )
	{
		BtStrCopy( pValue, size, m_current.m_value, 256 );
		BtStrTrimNewLines( pValue, size );
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

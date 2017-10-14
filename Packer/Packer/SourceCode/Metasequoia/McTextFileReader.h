////////////////////////////////////////////////////////////////////////////////
// __MCTextFileReader_H__

// Include guard
#ifndef __MCTextFileReader_H__
#define __MCTextFileReader_H__

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "BtString.h"
#include "McArray.h"

class LBaArchiveManager;

typedef BtChar BtString[256];

struct MCTextLine
{
	BtU32						m_first;
	BtU32						m_last;
};

// Class definition
class MCTextFileReader
{
public:

	// Constructor
	MCTextFileReader();

	void							Open( const BtChar* pFilename );
	void							Open( BtU8* pData, BtU32 dataSize );

	// Accessors
	BtU32							GetNumLines() const;
	BtU32							GetNumTokens( BtU32 line) const;
	const BtChar*					GetToken( BtU32 line, BtU32 token ) const;
	const BtChar*					GetTokenAsString( BtU32 line, BtU32 token );
	BtS32							GetTokenAsInteger( BtU32 line, BtU32 token ) const;
	BtFloat							GetTokenAsFloat( BtU32 line, BtU32 token ) const;
	const BtChar*					GetFullLine( BtU32 line );
	
	BtS32							GetBracktedExpression( BtU32 iLine, const BtChar* pToken );
	BtFloat							GetBracktedExpressionAsFloat( BtU32 iIndex );
	const BtChar*					GetBracktedExpressionAsString( BtU32 iIndex );

private:

	// Private functions
	void							AddBracktedToken( const BtChar* pToken );

	// Private members
	
	McArray< std::string, 10000000>	m_strings;
	McArray< MCTextLine,  10000000>	m_textLines;

	// Read bracketed expression
	McArray< std::string, 256>		m_values;

	BtChar							m_fullLine[256];
	BtChar							m_stringToken[256];
};

inline BtU32 MCTextFileReader::GetNumLines() const
{
	return m_textLines.GetNumItems();
}

#endif // __MCTextFileReader_H__

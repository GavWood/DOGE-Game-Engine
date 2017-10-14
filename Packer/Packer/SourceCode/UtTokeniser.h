////////////////////////////////////////////////////////////////////////////////
// UtTokeniser.h

// Include guard
#ifndef __UtTokeniser_h__
#define __UtTokeniser_h__

#include <stdio.h>
#include "BtArray.h"
#include "BtTypes.h"

const BtU32 MaxTokenSize = 256;
const BtU32 MaxTokens = 256;

class UtToken
{
public:

	BtChar							m_name[MaxTokenSize];
	BtChar							m_value[MaxTokenSize];
	BtBool							m_bool;
};

class UtTokeniser
{
public:

	UtTokeniser();
	
	BtBool							Read( const BtChar* pName );
	BtBool							GetBool( const BtChar* pName, BtBool &result );
	BtBool							GetFloat( const BtChar* pName, BtFloat &result );
	BtBool							GetNum( const BtChar* pName, BtU32 &number );
	BtBool							GetString( const BtChar* pName, BtChar* pString, BtU32 size );
	BtBool							GetToken( const BtChar* pName );
	
protected:

	FILE*							m_file;
	UtToken							m_current;
	BtArray<UtToken, MaxTokens>		m_tokens;
	BtU32							m_numTokens;
};

#endif // __UtTokeniser_h__

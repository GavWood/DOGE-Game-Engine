////////////////////////////////////////////////////////////////////////////////
// BtString.cpp

#include "BtString.h"
#include "BtPrint.h"
#include "BtMemory.h"
#include "MtMath.h"
#include <string.h>
#include <stdio.h>
#include <cctype>

////////////////////////////////////////////////////////////////////////////////
// LBtStrLength

BtU32 BtStrLength( const BtChar* pString )
{
	return (BtU32) strlen( pString );
}

////////////////////////////////////////////////////////////////////////////////
// BtStrUpper

void BtStrUpper( BtChar *str, BtU32 nSize )
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}

////////////////////////////////////////////////////////////////////////////////
// BtStrEmpty

void BtStrEmpty(BtChar* pString, BtU32 nSize)
{
	BtMemory::Set(pString, 0, nSize);
	strcpy(pString, "");
}

////////////////////////////////////////////////////////////////////////////////
// BtStrLower

void BtStrLower( BtChar *str, BtU32 nSize )
{
    while (*str)
    {
        *str = tolower(*str);
        str++;
    }
}

////////////////////////////////////////////////////////////////////////////////
// BtStrCompare

BtBool BtStrCompare( const BtChar* pString1, const BtChar* pString2 )
{	
	if( strcmp( pString1, pString2 ) == 0 )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// LBtStrCompare

BtBool BtStrCompare( const BtChar* pString1, const BtChar* pString2, BtU32 length )
{	
	if( BtMemory::Compare( (void*)pString1, (void*)pString2, length ) == BtTrue )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// LBtStrCompareNoCase

BtBool BtStrCompareNoCase( const BtChar* pString1, const BtChar* pString2 )
{
	BtChar string1[256];
	BtChar string2[256];
	
	BtStrCopy( string1, 256, pString1 );
	BtStrCopy( string2, 256, pString2 );

	BtStrLower( string1, 256 );
	BtStrLower( string2, 256 );

	if( BtStrCompare( string1, string2 ) == BtTrue )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// LBtStringReplace

BtChar* LBtStringReplace( const BtChar* szSource,
						  BtChar* szDest,
						  BtU32 nDestSize,
						  const BtChar* szSearchString,
						  const BtChar *szReplaceString )
{
	*szDest = 0;

	BtChar* pDest = szDest;

	const BtChar* pCurrent = szSource;

	while( pCurrent != BtNull )
	{
		const BtChar* pStart = strstr( pCurrent, szSearchString );

		if( pStart )
		{
			BtU32 nLength = (BtU32) ( pStart - pCurrent );
			if( nLength > 0 )
			{
				BtU32 nNewDestSize = (BtU32)( szDest + nDestSize - pDest );
				BtStrCopy( pDest, nNewDestSize, pCurrent, nLength );
				pDest+= nLength;
			}
			BtU32 nNewDestSize = (BtU32)( szDest + nDestSize - pDest );
			BtStrCopy( pDest, nNewDestSize, szReplaceString );
			pDest+=strlen(szReplaceString);
			pCurrent = pStart + strlen( szSearchString );
		}
		else
		{
			BtU32 nNewDestSize = (BtU32)( szDest + nDestSize - pDest );
			BtStrCopy( pDest, nNewDestSize, pCurrent );
			pCurrent = BtNull;
		}
	}

	return szDest;
}

////////////////////////////////////////////////////////////////////////////////
// BtStrCopy

void BtStrCopy(BtChar* szDestination, const BtChar* szSource)
{
	strcpy(szDestination, szSource);
}

////////////////////////////////////////////////////////////////////////////////
// BtStrCopy

void BtStrCopy( BtChar* szDestination, BtU32 nDestSize, const BtChar* szSource )
{
	strcpy( szDestination, szSource );
}

////////////////////////////////////////////////////////////////////////////////
// BtStrCopy

void BtStrCopy( BtChar* szDestination, BtU32 nDestSize, const BtChar* szSource, BtU32 nSize )
{
	strcpy( szDestination, szSource );
}

////////////////////////////////////////////////////////////////////////////////
// LBtStrCat

void BtStrCat( BtChar* pDestation, BtU32 nDestSize, const BtChar* pSource )
{
	strcat( pDestation, pSource );
}

////////////////////////////////////////////////////////////////////////////////
// LBtStrCat

void BtStrCat( BtChar* pDestation, BtU32 nDestSize, BtU32 numValue )
{
	BtChar temp[32];
	sprintf( temp, "%d", numValue );
	strcat( pDestation, temp);
}

////////////////////////////////////////////////////////////////////////////////
// BtStrStr

BtChar* BtStrStr( const BtChar* pSource, const BtChar* pToFind )
{
	return (BtChar*) strstr( pSource, pToFind );
}

////////////////////////////////////////////////////////////////////////////////
// BtStrRight

BtChar* BtStrRight(const BtChar* pSource, const BtChar toFind)
{
	return (BtChar*)strrchr(pSource, toFind);
}

////////////////////////////////////////////////////////////////////////////////
// LBtStrTrimNewLines

void LBtStrTrimSpaces( BtChar* szSource, BtU32 nSize )
{
	BtU32 length = BtStrLength( szSource );

	for( BtS32 i=length - 1; i >= 0; i-- )
	{
		BtChar& myChar = szSource[i];

		switch( myChar )
		{
			case ' ' :
			case '\n' :
			case '\r' :
			case '\0' :

				if( myChar == ' ' )
				{
					myChar = '\0';
				}
				break;

			default:
				i = -1;
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// LBtStrTrimNewLines

void LBtStrTrimNewLines( BtChar* szSource, BtU32 nSize )
{
	BtChar* pSource;

	pSource = strchr( szSource, '\n' );

	if( pSource )
	{
		*pSource = '\0';
	}

	pSource = strchr( szSource, '\r' );

	if( pSource )
	{
		*pSource = '\0';
	}
}

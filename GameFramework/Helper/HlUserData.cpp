////////////////////////////////////////////////////////////////////////////////
// HlUserData

// Includes
#include "HlUserData.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "BaUserData.h"
#include "FsFile.h"
#include "ApConfig.h"

// statics

////////////////////////////////////////////////////////////////////////////////
// GetSize

BtU32 HlUserData::GetSize( BaArchive *pArchive, const BtChar *name )
{
	// Get the length of data saved by the user
	BtU32 length = pArchive->GetUserData( name )->GetSize();

	// Return this length
	return length;
}

////////////////////////////////////////////////////////////////////////////////
// GetData

BtU8 *HlUserData::GetData( BaArchive *pArchive, const BtChar *name )
{
	BaUserData *pUserData = pArchive->GetUserData(name);

	if (pUserData)
	{
		// Get the data pointer
		BtU8 *pData = pUserData->GetData();

		// Return the data
		return pData;
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetXMLFromFile

BtBool HlUserData::GetXMLFromFile(tinyxml2::XMLDocument &document, BaArchive *pArchive, const BtChar *title)
{
	BtU32 length = 0;
	BtChar xmlString[1024 * 10];
	BtChar *pXML = xmlString;

	const BtChar *documentsPath = ApConfig::GetDocuments();
	BtChar filename[256];
	sprintf(filename, "%s%s.txt", documentsPath, title);
	length = FsFile::LoadString(filename, pXML);

	if( length )
	{
		tinyxml2::XMLError errorCode = document.Parse(pXML, length);
		if(errorCode == tinyxml2::XML_NO_ERROR)
		{
			return BtTrue;
		}
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetXML

BtBool HlUserData::GetXML( tinyxml2::XMLDocument &document, BaArchive *pArchive, const BtChar *title )
{
	BtU32 length = 0;
	BtChar xmlString[1024 * 10];
	BtChar *pXML = xmlString;

	const BtChar *resourcePath = ApConfig::GetResourcePath();
	BtChar filename[256];
	sprintf(filename, "%s%s.txt", resourcePath, title);
	length = FsFile::LoadString(filename, pXML);
	
 	if( length == 0 )
	{
		// Get the file from the archove
		BaUserData *pUserData = pArchive->GetUserData(title);

		if (pUserData)
		{
			pXML = (BtChar*)pUserData->GetData();
			length = pUserData->GetSize();
		}
	}

	tinyxml2::XMLError errorCode = document.Parse( pXML, length);
	if(errorCode == tinyxml2::XML_NO_ERROR)
	{
		return BtTrue;
	}
	BtPrint( document.GetErrorStr1() );
	return BtFalse;
}

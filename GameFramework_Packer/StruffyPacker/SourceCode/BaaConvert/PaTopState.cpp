////////////////////////////////////////////////////////////////////////////////
// main.cpp

#include <stdio.h>
#include <direct.h>
#include <tchar.h>
#include "BtString.h"
#include "BtTypes.h"
#include "DtClock.h"
#include "BtBase.h"
#include "EnBase.h"
#include "Application.h"
#include "BaArchive.h"
#include "RsRenderCore.h"
#include "PaTopState.h"
#include "ErrorLog.h"
#include "MCTextFileReader.h"
#include "UtFilename.h"
#include "BCMetasequoiaExport.h"
#include "FCollada.h"

// Defines
PaTopState s_GameState;

const BtFloat c_versionNumber = 1.10f;

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaTopState::PaTopState()
{
}

////////////////////////////////////////////////////////////////////////////////
// ConvertXYZToDAE

void PaTopState::ConvertXYZToDAE( UtFilename* pFilename )
{
	// Write the file we're converting
	ErrorLog::Printf( "Converting file %s to %s.dae\r\n\r\n", pFilename->GetFilename(), pFilename->GetTitle() );

	// Load the GetMapping file
	m_xyz.Load( pFilename, BtTrue );

	// Make the output filename
	BtChar outputFilename[256];

	BtStrEmpty( outputFilename, 256 );
	BtStrCat( outputFilename, 256, pFilename->GetTitle() );
	BtStrCat( outputFilename, 256, ".dae" );

	// Export the new filename
	m_collada.Create( m_metasequoia.GetVanilla() );
	m_collada.Export( outputFilename );
	m_collada.Destroy();
}

////////////////////////////////////////////////////////////////////////////////
// ConvertRawToDAE

void PaTopState::ConvertRawToMQO( UtFilename* pFilename )
{
	// Write the file we're converting
	ErrorLog::Printf( "Converting file %s to %s.dae\r\n\r\n", pFilename->GetFilename(), pFilename->GetTitle() );

	// Load the GetMapping file
	m_raw.Load( pFilename, BtFalse );

	// Make the output filename
	BtChar outputFilename[256];

	// Export as a Metasequoia file
	BtStrEmpty( outputFilename, 256 );
	BtStrCat( outputFilename, 256, pFilename->GetTitle() );
	BtStrCat( outputFilename, 256, ".mqo" );

	BCMetasequoiaExport metasequoia;
	metasequoia.Export( m_raw.GetVanilla(), outputFilename );
}

////////////////////////////////////////////////////////////////////////////////
// ConvertXYZToMQO

void PaTopState::ConvertXYZToMQO( UtFilename* pFilename )
{
	// Write the file we're converting
	ErrorLog::Printf( "Converting file %s to %s.dae\r\n\r\n", pFilename->GetFilename(), pFilename->GetTitle() );

	// Load the GetMapping file
	m_xyz.Load( pFilename, BtFalse );
	//m_xyz.Load( pFilename, BtTrue );

	// Make the output filename
	BtChar outputFilename[256];

	// Export as a Metasequoia file
	BtStrEmpty( outputFilename, 256 );
	BtStrCat( outputFilename, 256, pFilename->GetTitle() );
	BtStrCat( outputFilename, 256, ".mqo" );

	BCMetasequoiaExport metasequoia;
	metasequoia.Export( m_xyz.GetVanilla(), outputFilename );
}

////////////////////////////////////////////////////////////////////////////////
// ConvertDXFToDAE

void PaTopState::ConvertDXFToDAE( UtFilename* pFilename )
{
}

////////////////////////////////////////////////////////////////////////////////
// ConvertMQOToDAE

void PaTopState::ConvertMQOToDAE( UtFilename* pFilename )
{
	// Write the file we're converting
	ErrorLog::Printf( "Converting file %s to %s.dae\r\n\r\n", pFilename->GetFilename(), pFilename->GetTitle() );

	// Load the Metasequoia file
	m_metasequoia.Load( pFilename->GetFilename() );

	// Make the output filename
	BtChar outputFilename[256];

	BtStrEmpty( outputFilename, 256 );
	BtStrCat( outputFilename, 256, pFilename->GetTitle() );
	BtStrCat( outputFilename, 256, ".dae" );

	// Export the new filename
	m_collada.Create( m_metasequoia.GetVanilla() );
	m_collada.Export( outputFilename );
	m_collada.Destroy();
}

////////////////////////////////////////////////////////////////////////////////
// Main

void PaTopState::Main( int argc, _TCHAR* argv[] )
{
	BtChar pFilename1[256];
	BtChar pFilename2[256];

	// Cache the day
	BtU32 nDay = DtClock::Day();

	// Cache the month
	BtU32 nMonth = DtClock::Month();

	// Cache the year
	BtU32 nYear  = DtClock::Year();

	ErrorLog::Filename( "BaaConvert.txt" );
	ErrorLog::Create();

	ErrorLog::Printf( "*************BaaConvert*************\r\n\r\n" );

	BtChar detail[256];
	sprintf_s( detail, 256, "Version %.2f. Collada Command line convertor.\r\n\r\n", c_versionNumber ); 
	ErrorLog::Printf( detail );

	BtU32 licenceMonth = 9;
	BtU32 licenceYear  = 2009;

	if( ( nMonth >= 9 ) && ( nYear >= 2009 ) )
	{
		ErrorLog::Printf( "Software licence not valid for todays date.\r\n\r\n" );
	}
	else
	{
		ErrorLog::Printf( "Valid Software licence. This licence expires start of %d/%d.\r\n\r\n", licenceMonth, licenceYear );
	}

	if( argc < 2 )
	{
		ErrorLog::Printf( "Error:\tBaaConvert was run only %d parameters.\r\n\r\n", argc );
		ErrorLog::Printf( "Usage:\tBaaConvert YourModel.mqo\r\n\tConverts YourModel.mqo to YourModel.dae\r\n\r\n" );
		ErrorLog::Printf( "\tBaaConvert HeightMap.xyz Heightmap.mqo\r\n\tConverts xyz heightmap to mqo\r\n\r\n" );
		ErrorLog::Printf( "\tBaaConvert HeightMap.xyz Heightmap.dae\r\n\tConverts xyz heightmap to dae\r\n\r\n" );
		ErrorLog::Printf( "\tBaaConvert HeightMap.raw Heightmap.dae\r\n\tConverts raw (16 bit intel byte order) heightmap to dae\r\n\r\n" );
		exit(0);
	}
	else
	{
		// Copy the filename
		BtStrCopy( pFilename1, 256,  argv[1] );

		// Construct a utility filename
		UtFilename filename1( pFilename1 );

		if( BtStrCompare( filename1.GetExtension(), "mqo" ) == BtTrue )
		{
			// Convert the MQO file to DAE
			ConvertMQOToDAE( &filename1 );
		}
		else if( BtStrCompare( filename1.GetExtension(), "raw" ) == BtTrue )
		{
			if( argc > 2 )
			{
				BtStrCopy( pFilename2, 256,  argv[2] );
				UtFilename filename2( pFilename2 );

				if( BtStrCompare( filename2.GetExtension(), "mqo" ) == BtTrue )
				{
					// Convert the XYZ file to MQO
					ConvertRawToMQO( &filename1 );
				}
			}
		}
		else if( BtStrCompare( filename1.GetExtension(), "xyz" ) == BtTrue )
		{
			if( argc > 2 )
			{
				BtStrCopy( pFilename2, 256,  argv[2] );
				UtFilename filename2( pFilename2 );

				if( BtStrCompare( filename2.GetExtension(), "mqo" ) == BtTrue )
				{
					// Convert the XYZ file to MQO
					ConvertXYZToMQO( &filename1 );
				}
				else
				{
					// Convert the MQO file to DAE
					ConvertXYZToDAE( &filename1 );
				}
			}
		}
	}

	// Write the log
	ErrorLog::Printf( "BaaConvert completed %d-%d-%d.\n", nDay, nMonth, nYear );
}

////////////////////////////////////////////////////////////////////////////////
// _tmain

int _tmain(int argc, _TCHAR* argv[])
{
	FCollada::Initialize();

	s_GameState.Main( argc, argv );

	FCollada::Release();

	return 0;
}

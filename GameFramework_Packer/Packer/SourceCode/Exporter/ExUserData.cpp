////////////////////////////////////////////////////////////////////////////////
// ExUserData

// Includes
#include "BtTypes.h"
#include "BtMemory.h"
#include "BaArchive.h"
#include "ExUserData.h"
#include "BaArchive.h"
#include "BaUserData.h"

////////////////////////////////////////////////////////////////////////////////
// Export

void ExUserData::Export()
{
	// Read the resource in
	PaMemoryResource memoryResource( PMR_Read );
	memoryResource.Load( GetFilename() );

	BtU32 dataSize = memoryResource.GetCurrentSize();

	BtU32 nullWord = 0;
	
	// Write the resource out
	PaMemoryResource resource( PMR_Write );
	resource << dataSize;
	resource << PaMemoryItem( memoryResource.pCurrent(), dataSize );
	resource << nullWord;

	AddResource( resource, GetFilename(), GetTitle(), BaRT_UserData );
}

////////////////////////////////////////////////////////////////////////////////
/// RsFlash.h

#pragma once
#include "BaResource.h"
#include "MtVector2.h"
#include "RsColour.h"

// Class Declaration
class RsFlash : public BaResource
{
	DECLARE_RESOURCETYPE( BaRT_Flash );

public:

	virtual void Reset() = 0;

	
	virtual void Update( BtFloat tick ) = 0;

	virtual void Render( const MtVector2& v2Position, BtU32 sortOrder ) = 0;

	// Accessors
	virtual BtU32 GetFrameNumber() = 0;
	virtual void SetLooping( BtBool isLooping ) = 0;
	virtual BtBool GetLooping() = 0;
	virtual BtBool IsFinished() = 0;
};

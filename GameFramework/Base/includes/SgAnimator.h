///////////////////////////////////////////////////////////////////////////////
// SgAnimator.h

#pragma once
#include "BaResource.h"
#include "BaArchive.h"
#include "SgNode.h"

// Class declaration
class SgAnimator : public BaResource
{
public:
	
	DECLARE_RESOURCETYPE( BaRT_Animator );

	virtual void						FixPointers(BtU8 *pFileData, BaArchive *pArchive) = 0;
	virtual void						Reset() = 0;
	virtual void						Update(SgSkin* pSkin) = 0;

	// Accessors
	virtual BtBool						IsEnded() const = 0;
	virtual BtBool						IsPaused() const = 0;
	virtual void						SetPaused(BtBool isPaused) = 0;
	virtual void						SetTime(BtFloat time) = 0;
	virtual BtFloat						GetDuration() = 0;
	virtual BtU32						GetNumFrames() = 0;
	virtual BtFloat						GetFrameRate() = 0;
};

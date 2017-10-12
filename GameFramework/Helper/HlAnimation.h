////////////////////////////////////////////////////////////////////////////////
// HlAnimation.h

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "BtQueue.h"

class BaArchive;
class SgAnimator;

struct HlAnimationStage
{
	BtChar									name[32];
	BtU32									start;
	BtU32									end;
};

struct HlAnimationQueue
{
	HlAnimationStage					   *m_pAnimationStage;
};

// Class definition
class HlAnimation
{
public:
	
	// Public functions
	void									Setup( BaArchive *pArchive, const BtChar *name);
	void									Reset( const BtChar *name);
	void									SetAnimation(const BtChar *name);
	void									SetAnimationOnDifferent( const BtChar *name);
	void									AddToQueue( const BtChar *name);
	void									Update();

	// Accessors
	BtU32									GetAnimationFrame();
	BtU32									GetAnimationPosition();
	BtBool									GetNumQueuedAnimations();
	HlAnimationStage					   *GetCurrentAnimation();
	SgAnimator							   *GetAnimator() { return m_pAnimator; }

private:

	BtArray< HlAnimationStage, 256>			m_animations;
	BtQueue< HlAnimationQueue, 256>			m_queue;

	HlAnimationStage					   *m_pCurrentAnimation;

	BtU32									m_frame;
	BtU32									m_position;
	BtU32									m_tickCount;
	SgAnimator							   *m_pAnimator;
};

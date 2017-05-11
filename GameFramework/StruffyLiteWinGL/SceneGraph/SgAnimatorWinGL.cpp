
///////////////////////////////////////////////////////////////////////////////
// SgAnimatorWinDX.h

// Includes
#include "BtTypes.h"
#include "BtTime.h"
#include "SgAnimatorWinGL.h"
#include "SgNodeWinGL.h"
#include "SgBoneWinGL.h"
#include "SgSkinWinGL.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor

SgAnimatorWinGL::SgAnimatorWinGL()
{
	m_bSetup = BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// pDuplicate

SgAnimator *SgAnimatorWinGL::pDuplicate()
{
	// Allocate the memory
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Remove

void SgAnimatorWinGL::Remove()
{
}

///////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgAnimatorWinGL::FixPointers( BtU8* pFileData, BaArchive* pArchive )
{
	// Set the file data
	m_pFileData = (BaSgAnimationFileData*) pFileData;

	BtU8* pCurrent = (BtU8*) m_pFileData;

	// Skip the animation file data
	pCurrent+= sizeof(BaSgAnimationFileData);

	// Set the animation blocks
	m_pTimes = (BaSgAnimationTimeFileData*) pCurrent;

	// Skip the animation file data
	pCurrent+= sizeof(BaSgAnimationTimeFileData) * m_pFileData->m_maxFrames;
	
	// Set the keys
	m_pTransforms = (BaSgAnimationTransformFileData*) pCurrent;

	// Mark up the animations for the main scene hierarchy
	for (BtU32 frame = 0; frame < m_pFileData->m_maxFrames;frame++ )
	{
		// Cache each time
		const BaSgAnimationTimeFileData& time = m_pTimes[frame];

		for (BtU32 iTransform = time.m_iTransformStart; iTransform < time.m_iTransformEnd; iTransform++)
		{
			// Cache each transform
			BaSgAnimationTransformFileData& transform = m_pTransforms[iTransform];

			if (transform.m_iBone == -1)
			{
				transform.m_pNode = (SgNode*)pArchive->GetResource(transform.m_nodeIndex);
				int a = 0;
				a++;
			}
		}
	}

	int a=0;
	a++;
}

///////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void SgAnimatorWinGL::CreateOnDevice()
{
}

///////////////////////////////////////////////////////////////////////////////
// Reset

void SgAnimatorWinGL::Reset()
{
	m_time = 0;
	m_bEnded  = BtFalse;
	m_isPaused = BtFalse;
}

void SgAnimatorWinGL::SetPaused( BtBool bPause )
{
	m_isPaused = bPause;
}

BtBool SgAnimatorWinGL::IsEnded() const
{
	return m_bEnded;
}

BtBool SgAnimatorWinGL::IsPaused() const
{
	return m_isPaused;
}

///////////////////////////////////////////////////////////////////////////////
// GetDuration

BtFloat SgAnimatorWinGL::GetDuration()
{
	return m_pFileData->m_duration;
}

///////////////////////////////////////////////////////////////////////////////
// GetNumFrames

BtU32 SgAnimatorWinGL::GetNumFrames()
{
	return m_pFileData->m_maxFrames;
}

///////////////////////////////////////////////////////////////////////////////
// SetTime

void SgAnimatorWinGL::SetTime( BtFloat animTime )
{
	m_time = animTime;
}

///////////////////////////////////////////////////////////////////////////////
// GetTime

BtFloat SgAnimatorWinGL::GetTime()
{
	return m_time;
}

///////////////////////////////////////////////////////////////////////////////
// SetFrame

void SgAnimatorWinGL::SetFrame( BtU32 frame )
{
	m_time = ((BtFloat)frame) / m_pFileData->m_frameRate;
}

///////////////////////////////////////////////////////////////////////////////
// GetFrameRate

BtFloat SgAnimatorWinGL::GetFrameRate()
{
	return 1.0f / m_pFileData->m_frameRate;
}

///////////////////////////////////////////////////////////////////////////////
// Animate

void SgAnimatorWinGL::Update( SgSkin* pSkin )
{
	if( m_isPaused == BtTrue )
	{
		return;
	}

	// Cache the windows skin
	SgSkinWinGL *pSkinWinGL = (SgSkinWinGL*)pSkin;

	BtU32 frame = (BtU32)( m_time / GetFrameRate() );

	// Is this frame valid
	if( frame < m_pFileData->m_maxFrames )
	{
		// Cache each time
		const BaSgAnimationTimeFileData& time = m_pTimes[frame];

		for( BtU32 iTransform=time.m_iTransformStart; iTransform<time.m_iTransformEnd; iTransform++ )
		{
			// Cache each transform
			const BaSgAnimationTransformFileData& transform = m_pTransforms[iTransform];

			if (transform.m_iBone != -1)
			{
				// Do it the fast way
				SgNode* pNode = (SgNode*)pSkinWinGL->m_pFileData->m_skeleton[transform.m_iBone].m_pJoint;
				pNode->SetLocalTransform(transform.m_m4Transform);
			}
			else
			{
				// Do it the fast way
				SgNode* pNode = (SgNode*)transform.m_pNode;
				pNode->SetLocalTransform(transform.m_m4Transform);
			}

			int a = 0;
			a++;
		}
	}
	else
	{
		// Flag the animation as ended
		m_bEnded = BtTrue;
	}
}

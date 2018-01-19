
///////////////////////////////////////////////////////////////////////////////
// SgAnimatorImpl.h

// Includes
#include "BtTypes.h"
#include "BtTime.h"
#include "SgAnimatorImpl.h"
#include "SgNodeImpl.h"
#include "SgBoneImpl.h"
#include "SgSkinImpl.h"
#include "BtMemory.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor

SgAnimatorImpl::SgAnimatorImpl()
{
	m_bSetup = BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// GetDuplicate

SgAnimator *SgAnimatorImpl::GetDuplicate()
{
	// Allocate the memory
	BtU8* pMemory = BtMemory::Allocate(sizeof(SgAnimatorImpl));

	// Create the class
	SgAnimatorImpl* pAnimator = new(pMemory) SgAnimatorImpl;

	// Copy the memory
	BtMemory::Copy(pMemory, this, sizeof(SgAnimatorImpl));

	// Flag as a duplicate
	pAnimator->m_isDuplicate = BtTrue;

	// Add the duplicate
	m_pArchive->AddDuplicate(pAnimator);

	// Return the memory
	return pAnimator;
}

////////////////////////////////////////////////////////////////////////////////
// Remove

void SgAnimatorImpl::Remove()
{
}

///////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgAnimatorImpl::FixPointers( BtU8* pFileData, BaArchive* pArchive )
{
	// Set the archive
	m_pArchive = pArchive;

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
                transform.m_pNode = (SgNode*)pArchive->GetResource((BtU32)transform.m_nodeIndex);
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

void SgAnimatorImpl::CreateOnDevice()
{
}

///////////////////////////////////////////////////////////////////////////////
// Reset

void SgAnimatorImpl::Reset()
{
	m_time = 0;
	m_bEnded  = BtFalse;
	m_bPaused = BtFalse;
}

void SgAnimatorImpl::SetPaused( BtBool isPause )
{
	m_bPaused = isPause;
}

BtBool SgAnimatorImpl::IsEnded() const
{
	return m_bEnded;
}

BtBool SgAnimatorImpl::IsPaused() const
{
	return m_bPaused;
}

///////////////////////////////////////////////////////////////////////////////
// GetFrameRate

BtFloat SgAnimatorImpl::GetFrameRate()
{
    return 1.0f / m_pFileData->m_frameRate;
}

///////////////////////////////////////////////////////////////////////////////
// GetDuration

BtFloat SgAnimatorImpl::GetDuration()
{
	return (BtFloat)m_pFileData->m_duration;
}

///////////////////////////////////////////////////////////////////////////////
// GetNumFrames

BtU32 SgAnimatorImpl::GetNumFrames()
{
	return m_pFileData->m_maxFrames;
}

///////////////////////////////////////////////////////////////////////////////
// SetTime

void SgAnimatorImpl::SetTime( BtFloat animTime )
{
	m_time = animTime;
}

///////////////////////////////////////////////////////////////////////////////
// GetTime

BtFloat SgAnimatorImpl::GetTime()
{
	return m_time;
}

///////////////////////////////////////////////////////////////////////////////
// SetFrame

void SgAnimatorImpl::SetFrame( BtU32 frame )
{
	m_time = ((BtFloat)frame) / m_pFileData->m_frameRate;
}

///////////////////////////////////////////////////////////////////////////////
// Animate

void SgAnimatorImpl::Update( SgSkin* pSkin )
{
	if( m_bPaused == BtTrue )
	{
		return;
	}

	// Cache the windows skin
	SgSkinImpl *pSkinWinGL = (SgSkinImpl*)pSkin;

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

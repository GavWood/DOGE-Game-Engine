////////////////////////////////////////////////////////////////////////////////
// HlAnimation.cpp

// Includes
#include "HlUserData.h"
#include "HlAnimation.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "BaUserData.h"
#include "FsFile.h"
#include "ApConfig.h"
#include "SgAnimator.h"
#include "ErrorLog.h"
#include "UiKeyboard.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlAnimation::Setup( BaArchive *pArchive, const BtChar *name )
{
	m_animations.Empty();
	m_frame = 0;
	m_tickCount = 0;

	BaUserData *pUserData = pArchive->GetUserData(name);
	(void)pUserData;
	BtChar *string = (BtChar*)pUserData->GetData();
	BtU32 size = pUserData->GetSize();

	BtChar *pCurrent = string;
	BtChar *end;
	BtU32 length = 0;
	BtChar *pLast = string + size;

	while( pCurrent < pLast )
	{
		// extract the start number
		BtChar numbersStart[32];
		end = strstr(pCurrent, "-");
		length = (BtU32)(end - pCurrent);
		strncpy(numbersStart, pCurrent, length);
		*(numbersStart + length) = 0;
		pCurrent = pCurrent + length + 1;

		// extract the end number
		BtChar numbersEnd[32];
		end = strstr(pCurrent, " ");
        length = (BtU32)(end - pCurrent);
		strncpy(numbersEnd, pCurrent, length);
		*(numbersEnd + length) = 0;
		pCurrent = pCurrent + length + 1;

		// extract the loop
		BtChar loop[32];
		end = strstr(pCurrent, " ");
        length = (BtU32)(end - pCurrent);
		strncpy(loop, pCurrent, length);
		*(loop + length) = 0;
		pCurrent = pCurrent + length + 1;

		// extract the name
		BtChar name[32];
		end = strstr(pCurrent, "\r\n");
		if( !end )
		{
			end = pLast;
		}
        length = (BtU32)(end - pCurrent);
		strncpy(name, pCurrent, length);
		*(name + length) = 0;
		pCurrent = pCurrent + length + 2;

		HlAnimationStage stage;
		strcpy(stage.name, name);
		stage.start = atoi(numbersStart);
		stage.end = atoi(numbersEnd);
		m_animations.Add(stage);

		int a = 0;
		a++;
	}

	m_pAnimator = pArchive->GetAnimator(name);

	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// GetAnimationPosition

BtU32 HlAnimation::GetAnimationPosition()
{
	return m_position;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumQueuedAnimations

BtBool HlAnimation::GetNumQueuedAnimations()
{
	return m_queue.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// GetCurrentAnimation

HlAnimationStage *HlAnimation::GetCurrentAnimation()
{
	return m_pCurrentAnimation;
}

////////////////////////////////////////////////////////////////////////////////
// GetAnimationFrame

BtU32 HlAnimation::GetAnimationFrame()
{
	return m_frame;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void HlAnimation::Reset( const BtChar *name)
{
	m_pCurrentAnimation = BtNull;
	m_queue.Empty();

	// Cache the number of animations
	BtU32 count = m_animations.GetNumItems();

	for (BtU32 i = 0; i < count; i++)
	{
		if (strcmp(m_animations[i].name, name) == 0)
		{
			// Find the next animation
			HlAnimationQueue queue;
			queue.m_pAnimationStage = &m_animations[i];

			// Add it to the queue
			m_queue.Push(queue);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetAnimationOnDifferent

void HlAnimation::SetAnimationOnDifferent(const BtChar *name)
{
	HlAnimationStage *pAnimation = GetCurrentAnimation();

	if ( pAnimation && BtStrCompare(pAnimation->name, name))
	{
		AddToQueue(name);
	}
	else
	{
		SetAnimation(name);
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetAnimation

void HlAnimation::SetAnimation( const BtChar *name )
{
	// Cache the number of animations
	BtU32 count = m_animations.GetNumItems();

	for (BtU32 i = 0; i < count; i++)
	{
		if (strcmp(m_animations[i].name, name) == 0)
		{
			// Find the next animation
			HlAnimationQueue animatiin;
			animatiin.m_pAnimationStage = &m_animations[i];

			// Clear the current animation
			m_pCurrentAnimation = BtNull;

			// Empty the queue ready for the animation
			m_queue.Empty();

			// Add it to the queue
			m_queue.Push(animatiin);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddToQueue

void HlAnimation::AddToQueue( const BtChar *name)
{
	// Cache the number of animations
	BtU32 count = m_animations.GetNumItems();

	for (BtU32 i = 0; i < count; i++)
	{
		if (strcmp(m_animations[i].name, name) == 0)
		{
			// Find the next animation
			HlAnimationQueue animation;
			animation.m_pAnimationStage = &m_animations[i];

			// Add it to the queue
			m_queue.Push(animation);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlAnimation::Update()
{
	if( ApConfig::IsPaused() )
	{
		return;
	}

	if( !m_pCurrentAnimation && m_queue.GetItemCount() )
	{
		HlAnimationQueue queue = m_queue.Pop();

		m_pCurrentAnimation = queue.m_pAnimationStage;

		// Restart the animation
		m_frame = m_pCurrentAnimation->start;
	}

	if( m_pCurrentAnimation )
	{
		BtFloat frameRate = m_pAnimator->GetFrameRate();
		BtU32 duration = m_pAnimator->GetDuration() / frameRate;
		BtU32 start = m_pCurrentAnimation->start;
		BtU32 end  = m_pCurrentAnimation->end;
		m_position = end - m_frame;

		// Debug with keys
		BtU32 shift = 1;
		static BtU32 increment = 1;
		
        BtBool isTesting = BtFalse;
		if( isTesting )
		{
			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LSHIFT))
			{
				shift = 3;
			}
			if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_RSHIFT))
			{
				shift = 3;
			}

			if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_SPACE))
			{
				if (!increment)
				{
					increment = 1;
				}
				else
				{
					increment = 0;
				}
			}
			else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_PERIOD))
			{
				m_frame += shift;

				if (m_frame > duration)
				{
					m_frame = 0;
				}
				increment = 0;
				m_pAnimator->SetTime(m_frame);
			}
			else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_COMMA) )
			{
				if (m_frame > 0)
				{
					m_frame -= shift;
				}
				else
				{
					m_frame = duration;
				}
				increment = 0;
				m_pAnimator->SetTime(m_frame);
			}
			else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_X))
			{
				m_frame += shift;

				if (m_frame > end )
				{
					m_frame = start;
				}
				increment = 0;
				m_pAnimator->SetTime(m_frame);
			}
			else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_Z))
			{
				if (m_frame > start )
				{
					m_frame -= shift;
				}
				else
				{
					m_frame = end;
				}
				increment = 0;
				m_pAnimator->SetTime(m_frame);
			}
		}

		if (increment == 1)
		{
			m_tickCount++;

			if (m_tickCount > 3)
			{
				m_tickCount = 0;
				m_frame = m_frame + increment;

				BtFloat startTime = start;
				BtFloat endTime = end;
				
				if (m_frame < startTime)
				{
					m_frame = startTime;
				}
				else if (m_frame > endTime)
				{
					m_frame = endTime;
					m_pCurrentAnimation = BtNull;
				}
				
				//ErrorLog::Printf("frame=%f\n", m_frame);
				m_pAnimator->SetTime(m_frame);
				int a = 0;
				a++;
			}
		}	
	}
}
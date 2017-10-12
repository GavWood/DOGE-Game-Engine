////////////////////////////////////////////////////////////////////////////////
// ExSgAnimation.h

#ifndef __ExSgAnimation_h__
#define __ExSgAnimation_h__

#include "MtAABB.h"
#include "BaArchive.h"
#include <vector>

class ExScene;
class ExMaterialBlock;
struct ExSgNode;

struct ExSgAnimationKey
{
	BtFloat	m_time;
	MtMatrix4 m_m4Transform;
};

class ExSgAnimation
{
public:

	ExSgAnimation( ExSgNode* pNode, ExScene *pScene );
	~ExSgAnimation();

	void											AddAnimation( const ExSgAnimationKey& key );
	void											CopyAttributes();
	void											ChangeCoordinateSystem();

	// Accessors
	const std::vector<ExSgAnimationKey>&			AnimationKeys() const;
	BtFloat											Duration() const;
	BtBool											IsEnded() const;
	BtU32											GetNumKeys() const;

private:

	ExSgNode*										m_pNode;
	ExScene*										m_pScene;

	std::vector<ExSgAnimationKey>					m_animationKeys;
	BtFloat											m_maxDuration;
	BtBool											m_ended;
};

// Accessors
inline const std::vector<ExSgAnimationKey>& ExSgAnimation::AnimationKeys() const
{
	return m_animationKeys;
}

inline BtFloat ExSgAnimation::Duration() const
{
	return m_maxDuration;
}

inline BtBool ExSgAnimation::IsEnded() const
{
	return m_ended;
}

#endif // __ExSgAnimation_h__

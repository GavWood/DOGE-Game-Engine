////////////////////////////////////////////////////////////////////////////////
// ExSgLight.h

#ifndef __ExSgLight_h__
#define __ExSgLight_h__

#include "BaArchive.h"
#include "SgLight.h"
#include <vector>

class ExScene;
class ExMaterialBlock;
struct ExSgNode;

class ExSgLight
{
public:

	ExSgLight( ExSgNode* pNode, ExScene *pScene );
	~ExSgLight();

	void							CopyAttributes();
	void							ChangeCoordinateSystem();

private:

	friend class ExScene;

	BtFloat							m_intensity;
	RsLightType						m_type;
	ExSgNode*						m_pNode;
	ExScene*						m_pScene;
	RsColour						m_colour;
	BtFloat							m_fallOffAngle;
	BtFloat							m_outerAngle;
};

#endif // __ExSgLight_h__

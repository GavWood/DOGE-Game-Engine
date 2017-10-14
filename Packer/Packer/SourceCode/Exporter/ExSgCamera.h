////////////////////////////////////////////////////////////////////////////////
// ExSgCamera.h

#ifndef __ExSgCamera_h__
#define __ExSgCamera_h__

#include "BaArchive.h"
#include <vector>

class ExScene;
class ExMaterialBlock;
struct ExSgNode;

class ExSgCamera
{
public:

	ExSgCamera( ExSgNode* pNode, ExScene *pScene );
	~ExSgCamera();

	void							CopyAttributes();
	void							ChangeCoordinateSystem();

private:

	friend class ExScene;

	ExSgNode*						m_pNode;
	ExScene*						m_pScene;
	BtFloat							m_fNearPlane;
	BtFloat							m_fFarPlane;
	BtFloat							m_fFieldOfView;
};

#endif // __ExSgCamera_h__

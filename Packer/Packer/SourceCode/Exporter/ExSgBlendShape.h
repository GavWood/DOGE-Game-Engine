////////////////////////////////////////////////////////////////////////////////
// ExSgBlendShape.h

#ifndef __ExSgBlendShape_h__
#define __ExSgBlendShape_h__

#include "BaArchive.h"
#include <vector>

class ExScene;
class ExSgMesh;
struct ExSgNode;

class ExSgBlendShape
{
public:

	ExSgBlendShape( ExSgNode* pNode, ExScene *pScene );
	~ExSgBlendShape();

	void							CopyAttributes();
	void							ChangeCoordinateSystem();
	void							AddBlendShape( ExSgNode* pNode );
	void							CopyBlendShapes();
	BtU32							NumBlendShapes() const;

private:

	friend class ExScene;

	ExSgNode*						m_pNode;
	ExSgMesh*						m_pMesh;
	ExScene*						m_pScene;
	BtU32							m_nItems;

	std::vector<ExSgNode*>			m_pBlendShapes;
};

////////////////////////////////////////////////////////////////////////////////
// NumBlendShapes

inline BtU32 ExSgBlendShape::NumBlendShapes() const
{
	return (BtU32) m_pBlendShapes.size();
}

#endif // __ExSgBlendShape_h__

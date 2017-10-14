////////////////////////////////////////////////////////////////////////////////
// ExStrip.h

#pragma once
#include "BaArchive.h"
#include "BtCRC.h"
#include "ExVertex.h"
#include <vector>
#include <list>

class ExMaterialBlock;
class ExRenderBlock;

class ExGeometryOptimise
{
public:
	
	ExGeometryOptimise( ExMaterialBlock* pMaterialBlock, ExRenderBlock* pRenderBlock );

	void 							MergeVertex();
	void 							Strip();
	void							CopyVertexNoMerge();

private:

	ExMaterialBlock*				m_pMaterialBlock;
	ExRenderBlock*					m_pRenderBlock;
    std::list<ExVertex*>            m_vertexMap;
};

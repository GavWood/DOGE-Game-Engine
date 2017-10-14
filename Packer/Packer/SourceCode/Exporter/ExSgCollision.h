////////////////////////////////////////////////////////////////////////////////
// __ExCollision_h__

#ifndef __ExCollision_h__
#define __ExCollision_h__

#include "BaArchive.h"
#include "BtTypes.h"
#include "PaMemoryResource.h"
#include "ExResource.h"
#include "BtCRC.h"

class ExScene;
struct ExSgNode;
class ExSgMesh;

class ExSgCollision : public ExResource
{
public:

	ExSgCollision();
	ExSgCollision( ExSgNode* pNode, ExScene *pScene );

	void									Begin( BtU32 count );
	void									End( BtU32 count );

	void									ExtractCollisionMesh();

private:

	void									ExtractAnalyticalGeometry();

	BaSgCollisionFileData					m_fileData;

	ExSgNode*								m_pNode;
	ExSgMesh*								m_pMesh;
	ExScene*								m_pScene;
};

#endif // __ExCollision_h__

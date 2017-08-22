////////////////////////////////////////////////////////////////////////////////
// ExCollisionMesh

#pragma once
#include "BtTypes.h"
#include "ExResource.h"
#include "SgCollision.h"

class ExScene;
struct ExSgNode;
class ExSgMesh;

class ExCollisionMesh : public ExResource
{
public:

	ExCollisionMesh();
	void									ExtractMesh( ExSgNode* pNode );
	void									Export();

	// Accessors
	BtU32									NumIndex() const;
	BtU32									NumVertex() const;

private:
	
	std::vector<MtVector3>					m_vertices;
	std::vector<BtU32>						m_indices;
	BaSgCollisionMesh						m_fileData;
	ExSgNode*								m_pNode;
	ExScene*								m_pScene;
};

////////////////////////////////////////////////////////////////////////////////
// MCCollada.h

// Include guard
#ifndef __MCCollada_h__
#define __MCCollada_h__

#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "McVanilla.h"
#include <vector>

class FCDocument;
class FCDGeometryPolygons;
class FCDGeometryMesh;
class FCDMaterial;
class FCDSceneNode;

// Class definition
class MCCollada
{
public:

	// Public functions
	MCCollada();

	void						Create( const MCVanilla* pVanilla );
	void						Export( BtChar* fileName );
	void						Destroy();
	FCDocument*					GetDocument();

private:

	void						Create();
	void						ExtractObject();
	void						ExtractMaterials();
	void						ExtractVertex( const MCObject& object );
	void						ExtractPolygons( const MCObject& object );

	FCDocument*					m_pDocument;
	FCDGeometryMesh*			m_pGeometryMesh;
	FCDSceneNode*				m_pVisualScene;
	FCDSceneNode*				pSceneNode;
	std::vector<FCDMaterial*>	m_pMaterials;
	const MCVanilla*			m_pVanilla;
};

inline FCDocument* MCCollada::GetDocument()
{
	return m_pDocument;
}

#endif // __MCCollada_h__

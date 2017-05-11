////////////////////////////////////////////////////////////////////////////////
// SbFire.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector2.h"
#include "SbCamera.h"
#include "SdSound.h"
#include "BtCollection.h"
#include "RsColour.h"
#include "SbParticle.h"
#include "RsVertex.h"

class RsTexture;
class RsShader;
class RsSprite;
class RsMaterial;
class SgNode;

const BtU32 MaxBubbles = 128;
const BtU32 MaxBubbleVerts = MaxBubbles * 6;

// Class definition
class SbBubbles
{
public:

	// Public functions
	void										Setup( BaArchive *pArchive );
	void										Reset();
	void										Update();
	void										Render();
	void										SetPosition( MtVector3 v3Position );
	void										SetEmitter( BtBool isEmitterOn ) { m_emitterOn = isEmitterOn; }
	void										SetTransform(MtMatrix4 m4Transform) { m_m4Transform = m4Transform; }
	void										SetDimension(MtVector3 v3Dimension) { m_v3Dimension = v3Dimension; }

	// Accessors

private:

	// Private functions

	// Private members
	
	// Resources
	BtCollection< SbParticle, MaxBubbles >		m_particles;
	BtFloat										m_emitterCount;
	RsShader								   *m_pShader;
	RsMaterial								   *m_pParticle;
	MtVector3									m_v3Dimension;
	MtMatrix4									m_m4Transform;
	MtVector3									m_v3Position;
	MtVector3									m_v3Gravity;
	BtBool										m_emitterOn;
	RsVertex3									m_v3Vertex[MaxBubbleVerts];
};

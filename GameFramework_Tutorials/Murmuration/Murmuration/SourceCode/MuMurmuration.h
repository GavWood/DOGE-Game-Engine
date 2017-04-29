////////////////////////////////////////////////////////////////////////////////
// MuMurmuration.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BtCollection.h"
#include "MtVector3.h"

class BaArchive;
class SgNode;
class RsShader;
class RsMaterial;
class RsCamera;

// Forward prototype for structures
struct SbStarling;

// Main structures
struct SbKestrel
{
	MtVector3 v3Pos;
	MtVector3 v3Vel;

	MtVector3 v3DeltaVel;
	BtFloat shortestDistance;
	SbStarling *pStarling;
	BtU32 colour;
};

struct SbStarling
{
	BtU32 index;

	MtVector3 v3Pos;
	MtVector3 v3Vel;
	MtVector3 v3Target;
	BtS32 x1, y1, z1;
	BtS32 x4, y4, z4;
	BtU32 colour;

	BtFloat cohesion;
	BtFloat separation;
	BtFloat alignment;
	BtFloat predatorAvoidFactor;

	BtArray<SbStarling*, 7>			neighbours;
};

// Class definition
class SbMurmuration
{
public:

	// Public functions
	void							Init();
	void							Setup( BaArchive *pArchive );
	void							Update();
	void							RenderToTexture();
	void							Render( RsCamera *pCamera );
	void							Destroy();
	void							Reset();

	// Accessors
	MtVector3						GetPosition();

private:

	// Private function
	void							UpdateFactors();

	// Private members
	RsMaterial					   *m_pWhite3;
	RsMaterial					   *m_pBird3;

	MtVector3						m_v3Target;

	RsShader					   *m_pShader;
	BtBool							m_isPaused;
};

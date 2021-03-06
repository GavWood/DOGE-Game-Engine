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

struct SbConfig
{
	BtFloat								SeparationFactor;
	BtFloat								NeighbourAlignFactor;
	BtFloat								CohesionFactor;

	BtFloat								StarlingWingSpan;
	BtFloat								PereguineWingSpan;

	BtFloat								LocalTargetFactor;
    
    BtFloat                             PredatorLocalTargetFactor;

	BtFloat								MinSpeed;
	BtFloat								MaxSpeed;

	BtFloat								PredatorAvoidedFactor;
	BtFloat								PredatorAvoidDistance;
	BtFloat								PredatorAttractedFactor;

	BtU32								m_numBoids;
	BtU32								NumPredators;
};

// Main structures
struct SbPereguine
{
	MtVector3 v3Pos;
	MtVector3 v3Vel;

	MtVector3 v3DeltaVel;
	BtFloat shortestDistance;
    
    BtFloat targetIndex;

	BtU32 colour;
};

struct SbStarling
{
	BtFloat decisionCount;

	MtVector3 v3Pos;
	MtVector3 v3Vel;
    MtVector3 v3NewVel;
    MtVector3 v3LastVel;
    
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
    
    void                            Load();
    void                            Save();
    
	// Accessors
	MtVector3						GetPosition();
    BtChar                         *GetConfigFilename();
    BtChar                         *GetPredatorFilename();
    BtChar                         *GetStarlingFilename();

private:

	// Private function
	void							UpdateFactors();
    void                            SaveData();
    
    BtChar                          m_predatorFilename[256];
    BtChar                          m_starlingFilename[256];
    BtChar                          m_configFilename[256];
    

	// Private members
	RsMaterial					   *m_pBird3;
	RsMaterial					   *m_pWhite3;

	RsShader					   *m_pShader;
	BtBool							m_isPaused;

	SbConfig						m_config;
};

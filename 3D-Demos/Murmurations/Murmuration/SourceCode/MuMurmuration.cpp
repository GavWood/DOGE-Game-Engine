////////////////////////////////////////////////////////////////////////////////
// MuMumuration.cpp

// Includes
#include "RsVertex.h"
#include "RsTexture.h"
#include "BaArchive.h"
#include "RsMaterial.h"
#include "SgNode.h"
#include "UiKeyboard.h"
#include "Ui360.h"
#include "RsShader.h"
#include "MuMurmuration.h"
#include "BtTime.h"
#include "HlFont.h"
#include "RsRenderTarget.h"
#include "HlModel.h"
#include "RsUtil.h"
#include "HlDraw.h"
#include "BtArray.h"
#include "ApConfig.h"
#include "HlDebug.h"
#include "MtMath.h"
#include "MtAABB.h"
#include "ErrorLog.h"
#include "HlKeyboard.h"
#include "FsFile.h"
#include <stdio.h>
#include <vector>

#include "nanoflann.hpp"

// Defines

// Starling normal flight speed 50-65knts
// Starling foraging speed 30-40knts

const BtFloat WorldSize = 128.0f;
const BtFloat HalfWorldSize = WorldSize / 2;
const BtFloat SmallNumberInTermsOfAStarling = 0.001f;    // 1mm

const BtU32 MaxBoids = 4096;
const BtU32 MaxPredators = 2;

// Global variables to place the birds
SbPereguine g_predators[MaxPredators];
SbStarling g_flock[MaxBoids];

MtAABB aabb;
const BtU32 MaxVerts = MaxBoids + MaxPredators;
RsVertex3 myVertex[MaxVerts * 3];
RsVertex3 PereguineVertex[MaxPredators * 3];

BtU32 starlingColour = RsColour(0.2f, 0.30f, 0.2f, 0.75f).asWord();

// ------------------------------ Variables --------------------------------

// Centre of flock calculated with an AABB sphere
MtVector3 g_v3Centre(0, 0, 0);
BtBool g_isSpotting = BtFalse;

//-------------------------------------------------------------
// KD Tree - this is used to calculate who the neighbours are

extern void doKDTree(SbStarling *pFlock, int numBoids);

//-------------------------------------------------------------

BtChar *SbMurmuration::GetConfigFilename()
{
    sprintf(m_configFilename, "%s%s", ApConfig::GetDocuments(), "config_osx.txt");
    return m_configFilename;
}

BtChar *SbMurmuration::GetPredatorFilename()
{
    sprintf(m_predatorFilename, "%s%s", ApConfig::GetDocuments(), "predators_osx.txt");
    return m_predatorFilename;
}

BtChar *SbMurmuration::GetStarlingFilename()
{
    sprintf(m_starlingFilename, "%s%s", ApConfig::GetDocuments(), "starlings_osx.txt");
    return m_starlingFilename;
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbMurmuration::Setup( BaArchive *pArchive )
{
	m_pShader = pArchive->GetShader( "shader" );
	m_pBird3  = pArchive->GetMaterial("bird3");					// The texture for the bird
	m_pWhite3 = pArchive->GetMaterial("white3");				// White texture

	// Some good stable low factors
	m_config.SeparationFactor = 0.05f;
	m_config.NeighbourAlignFactor = 0.3f;
	m_config.CohesionFactor = 0.3f;

    m_config.PredatorLocalTargetFactor = 0.2f;
	m_config.LocalTargetFactor = 0.2f;
	m_config.MinSpeed = 15.0f;
	m_config.MaxSpeed = 18.0f;

	m_config.PredatorAvoidedFactor = 10.0f;
	m_config.PredatorAvoidDistance = 20.0f;
	m_config.PredatorAttractedFactor = 2.0f;

	m_config.StarlingWingSpan = 0.42f;					// 37 to 42cm
	m_config.PereguineWingSpan = 1.2f;					// 74 to 120cm
    m_config.m_numBoids = MaxBoids;
	m_config.NumPredators = 1;
}

////////////////////////////////////////////////////////////////////////////////
// Load

void SbMurmuration::Load()
{
    // Load the config from the file
    FsFile file;
    file.Open( GetConfigFilename(), FsMode_Read);
    if (file.IsOpen())
    {
        if (file.GetSize() == sizeof(SbConfig) )
        {
            file.Read(m_config);
        }
        file.Close();
    }
    
    // Important - we can set MaxBoids so cap the config to match that
    // otherwise we might have saved more birds than are in the array
    m_config.m_numBoids = MtMin( m_config.m_numBoids, MaxBoids );
    m_config.NumPredators = MtMin( m_config.NumPredators, MaxPredators );

    // Read the bird positions
    file.Open( GetStarlingFilename(), FsMode_Read);
    if (file.IsOpen())
    {
        BtU32 maxSize = sizeof(SbStarling) * m_config.m_numBoids;
        if (file.GetSize() == maxSize)
        {
            for (BtU32 i = 0; i < m_config.m_numBoids; i++)
            {
                SbStarling &starling = g_flock[i];
                file.Read(starling);
            }
        }
        file.Close();
    }

    file.Open( GetPredatorFilename(), FsMode_Read);
    if (file.IsOpen())
    {
        BtU32 maxSize = sizeof(SbPereguine) * m_config.NumPredators;
        if (file.GetSize() == maxSize )
        {
            for (BtU32 i = 0; i < m_config.NumPredators; i++)
            {
                SbPereguine &Peregrine = g_predators[i];
                file.Read(Peregrine);
            }
            file.Close();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void SbMurmuration::Reset()
{
	RdRandom::SetSeed(0);

	m_isPaused = BtFalse;

	for(BtU32 i = 0; i < MaxPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];

		BtFloat x = RdRandom::GetFloat(-WorldSize, WorldSize);
		BtFloat y = RdRandom::GetFloat(-WorldSize, WorldSize);
		BtFloat z = RdRandom::GetFloat(-WorldSize, WorldSize);
		
		MtVector3 v3Position(x, y, z);
		Peregrine.v3Pos = v3Position;
		Peregrine.v3Vel = MtVector3(0, 0, 0);
	}

	for (BtU32 i = 0; i < MaxBoids; i++)
	{
		SbStarling &starling = g_flock[i];
		starling.v3Target = MtVector3(0, 0, 0);
		starling.v3Vel = MtVector3(0, 0, 0);
		starling.decisionCount = RdRandom::GetFloat(0.0f, 10.0f);

		// Give them a target
		BtFloat x = RdRandom::GetFloat(-HalfWorldSize, HalfWorldSize);
		BtFloat y = RdRandom::GetFloat(-HalfWorldSize, HalfWorldSize);
		BtFloat z = RdRandom::GetFloat(-HalfWorldSize, HalfWorldSize);
		starling.v3Target = MtVector3(x, y, z);
	}

    for (BtU32 i = 0; i < MaxBoids; i++)
    {
        SbStarling &starling = g_flock[i];
        BtFloat x = RdRandom::GetFloat(-WorldSize, WorldSize);
        BtFloat y = RdRandom::GetFloat(-WorldSize, WorldSize);
        BtFloat z = RdRandom::GetFloat(-WorldSize, WorldSize);
        starling.v3Pos = MtVector3( x, y, z );
	}

    Load();

	// Give the Peregrines a dark reddish colour
	for (BtU32 i = 0; i < MaxPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];
		Peregrine.colour = RsColour(0.3f, 0.1f, 0.05f, 0.9f).asWord();
        Peregrine.targetIndex = i * ( m_config.m_numBoids / MaxPredators );
	}

    for( BtU32 tri=0; tri<MaxVerts; tri +=3 )
    {
        myVertex[tri + 0].m_colour = starlingColour;
        myVertex[tri + 1].m_colour = starlingColour;
        myVertex[tri + 2].m_colour = starlingColour;
        myVertex[tri + 0].m_v2UV = MtVector2(0.0f, 0.0f);
        myVertex[tri + 1].m_v2UV = MtVector2(0.0f, 0.5f);
        myVertex[tri + 2].m_v2UV = MtVector2(0.5f, 0.0f);
    }
    
    for( BtU32 tri=0; tri<MaxPredators * 3; tri +=3 )
    {
        PereguineVertex[tri + 0].m_colour = starlingColour;
        PereguineVertex[tri + 1].m_colour = starlingColour;
        PereguineVertex[tri + 2].m_colour = starlingColour;
        PereguineVertex[tri + 0].m_v2UV = MtVector2(0.0f, 0.0f);
        PereguineVertex[tri + 1].m_v2UV = MtVector2(0.0f, 0.5f);
        PereguineVertex[tri + 2].m_v2UV = MtVector2(0.5f, 0.0f);
    }
}

////////////////////////////////////////////////////////////////////////////////
// UpdateFactors

void SbMurmuration::UpdateFactors()
{
	for (BtU32 i = 0; i < m_config.m_numBoids; i++)
	{
        // We can add a little randomness to each starling's behaviour if we want
		SbStarling &Starling = g_flock[i];
		Starling.cohesion   = m_config.CohesionFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
		Starling.separation = m_config.SeparationFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
		Starling.alignment  = m_config.NeighbourAlignFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
		Starling.predatorAvoidFactor = m_config.PredatorAvoidedFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
	}
}

////////////////////////////////////////////////////////////////////////////////
// SaveData

void SbMurmuration::SaveData()
{
    FsFile file;
   file.Open( GetConfigFilename(), FsMode_Write);
    if (file.IsOpen())
    {
        file.Write(m_config);
        file.Close();
    }
    file.Open( GetStarlingFilename(), FsMode_Write);
    if (file.IsOpen())
    {
        for (BtU32 i = 0; i < m_config.m_numBoids; i++)
        {
            SbStarling &starling = g_flock[i];
            file.Write( starling );
        }
        file.Close();
    }
    file.Open( GetPredatorFilename(), FsMode_Write);
    if (file.IsOpen())
    {
        for (BtU32 i = 0; i < m_config.NumPredators; i++)
        {
            SbPereguine &Peregrine = g_predators[i];
            file.Write(Peregrine);
        }
        file.Close();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMurmuration::Update()
{
    BtFloat dt = BtTime::GetTick();

	if (UiKeyboard::pInstance()->IsPressed(SaveCameraKey))
	{
        SaveData();
	}

	BtFloat small = 0;

	BtFloat maxFactor = 20.0f;
	BtFloat maxBigFactor = 200.0f;
	BtFloat maxSpeed = MtKnotsToMetersPerSecond(60.0f);
	BtFloat maxDist = WorldSize;

	BtBool readOnly = BtFalse;
	HlDebug::Reset();
	HlDebug::AddUInt(0, "Num boids", &m_config.m_numBoids, readOnly, 1, MaxBoids, 1);
	HlDebug::AddUInt(0, "Num predators", &m_config.NumPredators, readOnly, 0, 2, 1);
	
	HlDebug::AddBool(0, "Is spotting", &g_isSpotting, readOnly);
	
	HlDebug::AddFloat(0, "Min speed", &m_config.MinSpeed, readOnly, HLUnits_Knots, small, maxSpeed);
	HlDebug::AddFloat(0, "Max speed", &m_config.MaxSpeed, readOnly, HLUnits_Knots, small, maxSpeed);

	HlDebug::AddFloat(0, "Predator avoid dist", &m_config.PredatorAvoidDistance, readOnly, HLUnits_StandardIndex, small, maxDist);

	HlDebug::AddFloat(0, "Predator avoided factor", &m_config.PredatorAvoidedFactor, readOnly, HLUnits_StandardIndex, small, maxBigFactor);
	HlDebug::AddFloat(0, "Predator attracted factor", &m_config.PredatorAttractedFactor, readOnly, HLUnits_StandardIndex, small, maxBigFactor);
	HlDebug::AddFloat(0, "Local target factor", &m_config.LocalTargetFactor, readOnly, HLUnits_StandardIndex, small, maxFactor);
    HlDebug::AddFloat(0, "Predator Local target factor", &m_config.PredatorLocalTargetFactor, readOnly, HLUnits_StandardIndex, small, maxFactor);
	HlDebug::AddFloat(0, "Cohesion", &m_config.CohesionFactor, readOnly, HLUnits_StandardIndex,					 small, maxFactor);
	HlDebug::AddFloat(0, "Separation factor", &m_config.SeparationFactor, readOnly, HLUnits_StandardIndex,		 small, maxFactor);
	HlDebug::AddFloat(0, "Alignment factor", &m_config.NeighbourAlignFactor, readOnly, HLUnits_StandardIndex,	 small, maxFactor);

	UpdateFactors();

	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_P ) )
	{
		m_isPaused = !m_isPaused;
	}

	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_R ) )
	{
		Reset();
	}

	// Calculate the ABB for our flock
	MtAABB aabb = MtAABB( g_flock[0].v3Pos );
	for( BtU32 i=1; i<m_config.m_numBoids; i++ )
	{
        MtVector3 v3Position = g_flock[i].v3Pos;
        
		// Expand the AABB
		aabb.ExpandBy( v3Position );
	}

	// Set the centre
	g_v3Centre = aabb.Center();

	// If we are paused exit
	if( m_isPaused == BtTrue )
	{
		return;
	}

	// Make a KD tree
	doKDTree( g_flock, m_config.m_numBoids );

	// Make the starlings avoid the predators
	for(BtU32 i = 0; i < m_config.NumPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];

        Peregrine.targetIndex = Peregrine.targetIndex + BtTime::GetTick();
        
		for(BtU32 j = 0; j < m_config.m_numBoids; j++)
		{
			SbStarling &starling = g_flock[j];

			MtVector3 v3Distance = Peregrine.v3Pos - starling.v3Pos;

			BtFloat distance = v3Distance.GetLengthSquared();

			if( distance <  m_config.PredatorAvoidDistance *  m_config.PredatorAvoidDistance )
			{
				MtVector3 v3Delta = v3Distance.GetNormalise();
				starling.v3Vel -= v3Delta * starling.predatorAvoidFactor;					// run away
			}
		}
	}

	// Predator attract
    for(BtU32 i = 0; i < m_config.NumPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];

        SbStarling *pStarling = &g_flock[ (BtU32)Peregrine.targetIndex ];

        MtVector3 v3Distance = pStarling->v3Pos - Peregrine.v3Pos;
        if( v3Distance.GetLength() > SmallNumberInTermsOfAStarling )
        {
            Peregrine.v3Vel += v3Distance.GetNormalise() *  m_config.PredatorAttractedFactor;
        }
	
		// Cap the speed
		BtFloat mag = Peregrine.v3Vel.GetLength();
		if (mag < m_config.MinSpeed)
		{
			Peregrine.v3Vel = Peregrine.v3Vel.Normalise() *  m_config.MinSpeed;
		}
		if(mag > m_config.MinSpeed)
		{
			Peregrine.v3Vel = Peregrine.v3Vel.Normalise() * m_config.MaxSpeed;
		}
        
        // Integrate the position
        Peregrine.v3Pos += Peregrine.v3Vel * dt;
	}

	// Main flocking calculations
	for( BtU32 i=0; i<m_config.m_numBoids; i++ )
	{
		SbStarling &bird = g_flock[i];
		bird.decisionCount -= BtTime::GetTick();

		if (bird.decisionCount < 0)
		{
			bird.decisionCount = RdRandom::GetFloat(5.0f, 10.0f);

			BtFloat x = RdRandom::GetFloat(-HalfWorldSize, HalfWorldSize);
			BtFloat y = RdRandom::GetFloat(-HalfWorldSize, HalfWorldSize);
			BtFloat z = RdRandom::GetFloat(-HalfWorldSize, HalfWorldSize);
            (void)x;
            (void)y;
            (void)z;
            
			// Do we use these fun local targets
			bird.v3Target = MtVector3(x, y, z);
            
            // Or a more global target
			//bird.v3Target = MtVector3(0, 0, 0);

			int a = 0;
			a++;
		}

		BtU32 numNeighbours = bird.neighbours.GetNumItems();
		if( numNeighbours )
		{
			// Reynolds rules from wikipedia
			//	separation: steer to avoid crowding local flockmates
			//	cohesion : steer to move toward the average position(center of mass) of local flockmates
			//	alignment : steer towards the average heading of local flockmates
            BtU32 neighboursUsed = 0;
			MtVector3 v3AverageVelocity(0, 0, 0);
			MtVector3 v3AveragePosition(0, 0, 0);

			for (BtU32 j = 0; j < numNeighbours; j++)
			{
				SbStarling *pNeighbour = bird.neighbours[j];
                
                MtVector3 v3One = pNeighbour->v3Pos - bird.v3Pos;
                MtVector3 v3Two = bird.v3Vel;
                BtFloat dp = v3One.DotProduct(v3Two);
                if( dp > 0 )
                {
                    v3AveragePosition += pNeighbour->v3Pos;
                    v3AverageVelocity += pNeighbour->v3Vel;
                    neighboursUsed++;
                }
			}
            if( neighboursUsed )
            {
                v3AveragePosition /= (BtFloat)neighboursUsed;
                v3AverageVelocity /= (BtFloat)neighboursUsed;
            }
            
			// Separate the birds
            for (BtU32 j = 0; j < numNeighbours; j++)
            {
                SbStarling *pNeighbour = bird.neighbours[j];

				MtVector3 v3Delta = pNeighbour->v3Pos - bird.v3Pos;
				if( v3Delta.GetLengthSquared() > SmallNumberInTermsOfAStarling )
				{
					bird.v3Vel -= v3Delta.GetNormalise() * bird.separation;
				}
			}

			// Cohese the birds
			{
				MtVector3 v3Delta = v3AveragePosition - bird.v3Pos;
				if( v3Delta.GetLengthSquared() > SmallNumberInTermsOfAStarling )
				{
					bird.v3Vel += v3Delta.GetNormalise() * bird.cohesion;
				}
			}

			// Align the birds
			{
				MtVector3 v3Delta = v3AverageVelocity - bird.v3Vel;
				if( v3Delta.GetLengthSquared() > SmallNumberInTermsOfAStarling )
				{
					bird.v3Vel += v3Delta.GetNormalise() * bird.alignment;
				}
			}
		}
		
	}

	// Update the birds velocity and position
	for( BtU32 i=0; i<m_config.m_numBoids; i++ )
	{
		SbStarling &starling = g_flock[i];

		{
			// Calculate the distance to the origin
			MtVector3 v3Delta = starling.v3Target - starling.v3Pos;

			if (v3Delta.GetLength() > 0)
			{
				// Update the velocity to the target
				starling.v3Vel += v3Delta *  m_config.LocalTargetFactor;
			}
		}

		// Cap the speed
		BtFloat mag = starling.v3Vel.GetLength();

		if (mag <  m_config.MinSpeed)
		{
			//starling.v3Vel = starling.v3Vel.Normalise() *  m_config.MinSpeed;
		}

		if( mag >  m_config.MaxSpeed )
		{
			starling.v3Vel = starling.v3Vel.Normalise() *  m_config.MaxSpeed;
		}
        
        // Calculat the new velocity
        starling.v3NewVel = starling.v3Vel;
        
        // Integrate the position
        starling.v3Pos += starling.v3NewVel * dt;
        
        // Store the old velocity
        starling.v3LastVel = starling.v3Vel;
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderToTexture

void SbMurmuration::RenderToTexture()
{
	// Render the shadow to the texture
	//m_shadow.RenderToTexture();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbMurmuration::Render( RsCamera *pCamera )
{
	// Setup the shader
	MtVector3 v3LightDirection(0, 1, 0);
	MtVector3 ambient(0.1f, 0.1f, 0.1f);
	m_pShader->SetFloats(RsHandles_Light0Direction, &v3LightDirection.x, 3);
	m_pShader->SetFloats(RsHandles_LightAmbient, &ambient.x, 3);

	// Cache the camera because we are going to use billboards to render the birds flat toward the camera
	MtMatrix4 m4View = pCamera->GetRotation();

	// Calculate what this flat vector would be
	MtVector3 v3Up(0, 1, 0);
	MtVector3 v3Side(1, 0, 0);
	v3Up *= m4View;
	v3Side *= m4View;

	// Setup the width of the birds. To save fill rate our "bird" is pictured in the corner
	// of our texture. This allows us to render a triangle instead of a quad
	// 
	//
	//  ______________
	// | \           |
	// |   \         |
	// |  _  \       |
	// | / \   \     |
	// |{   }    \   |
	// |_\_/______\__|

    // We then need to multiple the dimension by two as we are using half a triangle
    // Make these bigger to stand out in our sim (*3)
	const BtFloat StarlingHalfWingSpan  = m_config.StarlingWingSpan * 2.0f * 3.0f;
    const BtFloat PereguineHalfWingSpan = m_config.PereguineWingSpan  * 2.0f * 3.0f;

	MtMatrix3 m3Orientation = pCamera->GetRotation();
    MtVector3 v3AxisX = (m3Orientation.Col0() * StarlingHalfWingSpan);
    MtVector3 v3AxisY = (m3Orientation.Col1() * StarlingHalfWingSpan);
    
	// Draw the birds
	BtU32 tri = 0;
	if( m_config.m_numBoids )
	{
		for( BtU32 i=0; i<m_config.m_numBoids; i++ )
		{
			SbStarling &Starling = g_flock[i];

			MtVector3 &v3Position = Starling.v3Pos;

			// Render the bird
			myVertex[tri + 0].m_v3Position = v3Position;
			myVertex[tri + 1].m_v3Position = v3Position + v3AxisX;
			myVertex[tri + 2].m_v3Position = v3Position + v3AxisY;
			tri += 3;
		}

		if (g_isSpotting)
		{
			SbStarling &Starling = g_flock[0];

			MtVector3 &v3Position = Starling.v3Pos;

			BtU32 spotColour = RsColour::RedColour().asWord();

			// Render the spotted starling
			myVertex[0].m_v3Position = v3Position;
			myVertex[1].m_v3Position = v3Position + (m3Orientation.Col0() * StarlingHalfWingSpan * 2.0f );
			myVertex[2].m_v3Position = v3Position + (m3Orientation.Col1() * StarlingHalfWingSpan * 2.0f );
			myVertex[0].m_colour = spotColour;
			myVertex[1].m_colour = spotColour;
			myVertex[2].m_colour = spotColour;
		}
        else
        {
            myVertex[0].m_colour = starlingColour;
            myVertex[1].m_colour = starlingColour;
            myVertex[2].m_colour = starlingColour;
        }
		m_pBird3->Render(RsPT_TriangleList, myVertex, tri, MaxSortOrders - 1, BtFalse);
	}

	// Draw the predators
	if( m_config.NumPredators )
	{
        tri = 0;
		for(BtU32 i = 0; i < m_config.NumPredators; i++)
		{
			SbPereguine &Peregrine = g_predators[i];

			MtVector3 &v3Position = Peregrine.v3Pos;

			// Render the Peregrine
			PereguineVertex[tri + 0].m_v3Position = v3Position;
			PereguineVertex[tri + 1].m_v3Position = v3Position + (m3Orientation.Col0() * PereguineHalfWingSpan);
			PereguineVertex[tri + 2].m_v3Position = v3Position + (m3Orientation.Col1() * PereguineHalfWingSpan);
			tri += 3;
		}
		m_pBird3->Render(RsPT_TriangleList, PereguineVertex, m_config.NumPredators * 3, MaxSortOrders - 1, BtFalse);
    }

	if( ApConfig::IsDebug() )
	{
		HlDebug::Render();
        
        //BtChar text[32];
        //sprintf(text, "%.2f", g_predators[0].targetIndex );
        //HlFont::Render(MtVector2(200, 0), 3.0f, text, RsColour::BlackColour(), MaxSortOrders - 1);
        
		MtMatrix4 m4Transform;
		m4Transform.SetTranslation(g_v3Centre);
		HlDraw::RenderCross(m4Transform, 50.0f, MaxSortOrders - 1);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbMurmuration::Destroy()
{
}

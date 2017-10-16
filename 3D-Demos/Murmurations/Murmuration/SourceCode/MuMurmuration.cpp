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

// starling speed murmuration 
// traveling speed 50-65
// foraging speed 30-40

// Pre-requisits
const BtFloat WorldSize = 128;
const BtU32 MaxBoids = 2048.0f;
const BtU32 MaxPredators = 2;

MtAABB aabb;
const BtU32 MaxVerts = MaxBoids + MaxPredators;
RsVertex3 myVertex[MaxVerts * 3];
const BtU32 MaxNeighbours = 7;

// ------------------------------ Variables --------------------------------

// Centre of flock calculated with an AABB sphere
MtVector3 g_v3Centre(0, 0, 0);
BtBool g_isSpotting = BtFalse;

//-------------------------------------------------------------

// Global variables to place the birds
SbPereguine g_predators[MaxPredators];
SbStarling g_flock[MaxBoids];

//-------------------------------------------------------------
// KD Tree

extern void doKDTree(SbStarling *pFlock, int numBoids);

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbMurmuration::Setup( BaArchive *pArchive )
{
	m_pShader = pArchive->GetShader( "shader" );
	m_pBird3  = pArchive->GetMaterial("bird3");				// The texture for the bird

	// Some good stable low factors
	m_config.SeparationFactor = 0.05f;
	m_config.NeighbourAlignFactor = 0.3f;
	m_config.CohesionFactor = 0.3f;

	m_config.LocalTargetFactor = 0.2f;
	m_config.MinSpeed = 15.0f;
	m_config.MaxSpeed = 18.0f;

	m_config.PredatorAvoidedFactor = 10.0f;
	m_config.PredatorAvoidDistance = 20.0f;
	m_config.PredatorAttractedFactor = 2.0f;

	m_config.StarlingWingSpan = 0.37f;					// 37 to 42cm
	m_config.PereguineWingSpan = 1.2f;					// 74 to 120cm

	if (ApConfig::IsDebugBuild())
	{
		m_config.m_numBoids = 256;
	}
	else
	{
		m_config.m_numBoids = 2048;
	}
	m_config.NumPredators = 1;
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
		starling.decisionCount = RdRandom::GetNumber(0.0f, 10.0f);

		// Give them a target
		BtFloat x = RdRandom::GetFloat(-WorldSize, WorldSize);
		BtFloat y = RdRandom::GetFloat(-WorldSize, WorldSize);
		BtFloat z = RdRandom::GetFloat(-WorldSize, WorldSize);
		starling.v3Target = MtVector3(x, y, z);
	}

	if (1)
	{
		for (BtU32 i = 0; i < MaxBoids; i++)
		{
			SbStarling &starling = g_flock[i];
			BtFloat x = RdRandom::GetFloat(-WorldSize, WorldSize);
			BtFloat y = RdRandom::GetFloat(-WorldSize, WorldSize);
			BtFloat z = RdRandom::GetFloat(-WorldSize, WorldSize);
			starling.v3Pos = MtVector3(x, y, z);
		}
	}

	// Load the boids from the file
	FsFile file;
	BtChar filename[64];
	sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "config.txt");
	file.Open(filename, FsMode_Read);
	if (file.IsOpen())
	{
		if (file.GetSize() == sizeof(SbConfig) )
		{
			file.Read(m_config);
		}
		file.Close();
	}
	sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "boids.txt");
	file.Open(filename, FsMode_Read);
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
	sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "predators.txt");
	file.Open(filename, FsMode_Read);
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

	// Give the Peregrines a dark red colour
	for (BtU32 i = 0; i < MaxPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];
		Peregrine.colour = RsColour(0.3f, 0.1f, 0.05f, 0.5f).asWord();
	}

	// Give the starlings a random dark colour
	for (BtU32 i = 0; i < MaxBoids; i++)
	{
		SbStarling &starling = g_flock[i];
		BtFloat r = RdRandom::GetFloat(0.01f, 0.20f);
		BtFloat g = RdRandom::GetFloat(0.01f, 0.20f);
		BtFloat b = RdRandom::GetFloat(0.01f, 0.20f);
		starling.colour = RsColour(r, g, b, 0.5f).asWord();
	}
}

////////////////////////////////////////////////////////////////////////////////
// UpdateFactors

void SbMurmuration::UpdateFactors()
{
	for (BtS32 i = 0; i < m_config.m_numBoids; i++)
	{
		SbStarling &Starling = g_flock[i];
		Starling.cohesion = m_config.CohesionFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
		Starling.separation = m_config.SeparationFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
		Starling.alignment = m_config.NeighbourAlignFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
		Starling.predatorAvoidFactor = m_config.PredatorAvoidedFactor;//  *RdRandom::GetFloat(0.8f, 1.0f);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMurmuration::Update()
{
	BtFloat dt = BtTime::GetTick();

	if (UiKeyboard::pInstance()->IsPressed(SaveCameraKey))
	{
		FsFile file;
		BtChar filename[64];
		sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "config.txt");
		file.Open(filename, FsMode_Write);
		if (file.IsOpen())
		{
			file.Write(m_config);
			file.Close();
		}
		sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "boids.txt");
		file.Open(filename, FsMode_Write);
		if (file.IsOpen())
		{
			for (BtU32 i = 0; i < m_config.m_numBoids; i++)
			{
				SbStarling &starling = g_flock[i];
				file.Write( starling );
			}
			file.Close();
		}
		sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "predators.txt");
		file.Open(filename, FsMode_Write);
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

	BtFloat small = 0;

	BtFloat maxFactor = 20.0f;
	BtFloat maxBigFactor = 2.0f;
	BtFloat maxSpeed = MtKnotsToMetersPerSecond(60.0f);
	BtFloat maxDist = WorldSize;

	BtBool readOnly = BtFalse;
	HlDebug::Reset();
	HlDebug::AddUInt(0, "Num boids", &m_config.m_numBoids, readOnly, 1, MaxBoids, 1);
	HlDebug::AddInteger(0, "Num predators", &m_config.NumPredators, readOnly, 0, 2, 1);
	
	HlDebug::AddBool(0, "Is spotting", &g_isSpotting, readOnly);
	
	HlDebug::AddFloat(0, "Min speed", &m_config.MinSpeed, readOnly, HLUnits_Knots, small, maxSpeed);
	HlDebug::AddFloat(0, "Max speed", &m_config.MaxSpeed, readOnly, HLUnits_Knots, small, maxSpeed);

	HlDebug::AddFloat(0, "Predator avoid dist", &m_config.PredatorAvoidDistance, readOnly, HLUnits_StandardIndex, small, maxSpeed);

	HlDebug::AddFloat(0, "Predator avoided factor", &m_config.PredatorAvoidedFactor, readOnly, HLUnits_StandardIndex, small, maxBigFactor);
	HlDebug::AddFloat(0, "Predator attracted factor", &m_config.PredatorAttractedFactor, readOnly, HLUnits_StandardIndex, small, maxBigFactor);
	HlDebug::AddFloat(0, "Local target factor", &m_config.LocalTargetFactor, readOnly, HLUnits_StandardIndex, small, maxFactor);
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
	for( BtS32 i=1; i<m_config.m_numBoids; i++ )
	{
		// Expand the AABB
		aabb.ExpandBy( g_flock[i].v3Pos );
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
	for(BtS32 i = 0; i < m_config.NumPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];

		Peregrine.shortestDistance = (Peregrine.v3Pos - g_flock[0].v3Pos).GetLengthSquared();
		Peregrine.pStarling = &g_flock[0];

		for(BtS32 j = 0; j < m_config.m_numBoids; j++)
		{
			SbStarling &starling = g_flock[j];

			MtVector3 v3Distance = Peregrine.v3Pos - starling.v3Pos;

			BtFloat distance = v3Distance.GetLengthSquared();

			if( distance < Peregrine.shortestDistance )
			{
				Peregrine.shortestDistance = distance;
				Peregrine.pStarling = &starling;
			}
			if( distance <  m_config.PredatorAvoidDistance *  m_config.PredatorAvoidDistance )
			{
				MtVector3 v3Delta = v3Distance.GetNormalise();
				starling.v3Vel -= v3Delta * starling.predatorAvoidFactor;					// run away
			}
		}
	}

	// Predator attract
	for(BtS32 i = 0; i < m_config.NumPredators; i++)
	{
		SbPereguine &Peregrine = g_predators[i];

		MtVector3 v3PositionAboveFlock = g_v3Centre;
		//v3PositionAboveFlock.y = aabb.Max().y + 20.0f;

		if( Peregrine.shortestDistance <  m_config.PredatorAvoidDistance *  m_config.PredatorAvoidDistance )
		{
			//Peregrine.v3Pos.y = v3PositionAboveFlock.y;

			MtVector3 v3Distance = Peregrine.pStarling->v3Pos - Peregrine.v3Pos;
			if (v3Distance.GetLength())
			{
				Peregrine.v3Vel += v3Distance.GetNormalise() *  m_config.PredatorAttractedFactor;
			}
		}
		else
		{
			MtVector3 v3Distance = v3PositionAboveFlock - Peregrine.v3Pos;
			if (v3Distance.GetLength())
			{
				Peregrine.v3Vel += v3Distance.GetNormalise() *  m_config.PredatorAttractedFactor;
			}
		}

		// Integrate the position
		Peregrine.v3Pos += Peregrine.v3Vel * dt;

		// Cap the speed
		BtFloat mag = Peregrine.v3Vel.GetLength();
		if (mag <  m_config.MinSpeed)
		{
			Peregrine.v3Vel = Peregrine.v3Vel.Normalise() *  m_config.MinSpeed;
		}
		if(mag > m_config.MaxSpeed)
		{
			Peregrine.v3Vel = Peregrine.v3Vel.Normalise() * m_config.MaxSpeed;
		}
	}

	// Main flocking calculations
	for( BtS32 i=0; i<m_config.m_numBoids; i++ )
	{
		SbStarling &bird = g_flock[i];
		bird.decisionCount -= BtTime::GetTick();

		if (bird.decisionCount < 0)
		{
			bird.decisionCount = RdRandom::GetNumber(5.0f, 10.0f);

			BtFloat x = RdRandom::GetFloat(-WorldSize, WorldSize);
			BtFloat y = RdRandom::GetFloat(-WorldSize, WorldSize);
			BtFloat z = RdRandom::GetFloat(-WorldSize, WorldSize);

			// Fun local targets or global
//			bird.v3Target = MtVector3(x, y, z);
			bird.v3Target = MtVector3(0, 0, 0);

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

			MtVector3 v3AverageVelocity(0, 0, 0);
			MtVector3 v3AveragePosition(0, 0, 0);

			SbStarling *closestNeighbour = bird.neighbours[0];
			BtFloat closestDistance = (bird.neighbours[0]->v3Pos - bird.v3Pos).GetLengthSquared();

			for (BtU32 j = 0; j < numNeighbours; j++)
			{
				SbStarling *pNeighbour = bird.neighbours[j];

				v3AveragePosition = pNeighbour->v3Pos;
				v3AverageVelocity = pNeighbour->v3Vel;
			}
			v3AveragePosition /= (BtFloat)numNeighbours;
			v3AverageVelocity /= (BtFloat)numNeighbours;

			// Separate the birds
			{
				MtVector3 v3Delta = closestNeighbour->v3Pos - bird.v3Pos;
				if (v3Delta.GetLength())
				{
					bird.v3Vel -= v3Delta.GetNormalise() * bird.separation;
				}
			}

			// Cohese the birds
			{
				MtVector3 v3Delta = v3AveragePosition - bird.v3Pos;
				if (v3Delta.GetLength())
				{
					bird.v3Vel += v3Delta.GetNormalise() * bird.cohesion;
				}
			}

			// Align the birds
			{
				MtVector3 v3Delta = v3AverageVelocity - bird.v3Vel;
				if (v3Delta.GetLength() > m_config.MinSpeed * 0.1f )
				{
					bird.v3Vel += v3Delta.GetNormalise() * bird.alignment;
				}
			}
		}
		
	}

	// Update the birds velocity and position
	for( BtS32 i=0; i<m_config.m_numBoids; i++ )
	{
		SbStarling &starling = g_flock[i];

		{
			// Calculate the distance to the origin
			MtVector3 v3Delta = starling.v3Target - starling.v3Pos;

			if (v3Delta.GetLength() > 0)
			{
				// Update the velocity to the target
				starling.v3Vel += v3Delta.GetNormalise() *  m_config.LocalTargetFactor;
			}
		}

		// Integrate the position
		starling.v3Pos += starling.v3Vel * dt;

		// Cap the speed
		BtFloat mag = starling.v3Vel.GetLength();

		if (mag <  m_config.MinSpeed)
		{
			starling.v3Vel = starling.v3Vel.Normalise() *  m_config.MinSpeed;
		}

		if( mag >  m_config.MaxSpeed )
		{
			starling.v3Vel = starling.v3Vel.Normalise() *  m_config.MaxSpeed;
		}
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
	MtMatrix4 m4Projection = pCamera->GetProjection();
	MtMatrix4 m4ViewProjection = m4View * m4Projection;
	BtU32 width = (BtU32)pCamera->GetWidth();
	BtU32 height = (BtU32)pCamera->GetHeight();

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

	const BtFloat StarlingHalfWingSpan  = m_config.StarlingWingSpan * 12.0f;
	const BtFloat PereguineHalfWingSpan = m_config.PereguineWingSpan  * 12.0f;

	MtMatrix3 m3Orientation = pCamera->GetRotation();

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
			myVertex[tri + 1].m_v3Position = v3Position + (m3Orientation.Col0() * StarlingHalfWingSpan);
			myVertex[tri + 2].m_v3Position = v3Position + (m3Orientation.Col1() * StarlingHalfWingSpan);
			myVertex[tri + 0].m_colour = Starling.colour;
			myVertex[tri + 1].m_colour = Starling.colour;
			myVertex[tri + 2].m_colour = Starling.colour;

			myVertex[tri + 0].m_v2UV = MtVector2(0.0f, 0.0f);
			myVertex[tri + 1].m_v2UV = MtVector2(0.0f, 1.0f);
			myVertex[tri + 2].m_v2UV = MtVector2(1.0f, 1.0f);
			tri += 3;
		}

		for(BtU32 i = 0; i < tri; i += 3)
		{
			myVertex[i + 0].m_v2UV = MtVector2(0, 0);
			myVertex[i + 1].m_v2UV = MtVector2(0, 1);
			myVertex[i + 2].m_v2UV = MtVector2(1, 0);
		}

		if (g_isSpotting)
		{
			SbStarling &Starling = g_flock[0];

			MtVector3 &v3Position = Starling.v3Pos;

			BtU32 spotColour = RsColour::RedColour().asWord();

			// Render the spotted starling
			myVertex[0].m_v3Position = v3Position;
			myVertex[1].m_v3Position = v3Position + (m3Orientation.Col0() * StarlingHalfWingSpan);
			myVertex[2].m_v3Position = v3Position + (m3Orientation.Col1() * StarlingHalfWingSpan);
			myVertex[0].m_colour = spotColour;
			myVertex[1].m_colour = spotColour;
			myVertex[2].m_colour = spotColour;
		}
		
		m_pBird3->Render(RsPT_TriangleList, myVertex, tri, MaxSortOrders - 1, BtFalse);
	}

	// Draw the predators
	RsVertex3 *pVertex = &myVertex[tri];

	if(m_config.NumPredators)
	{
		const BtFloat PeregrineHalfWingSpan = m_config.PereguineWingSpan;

		for(BtS32 i = 0; i < m_config.NumPredators; i++)
		{
			SbPereguine &Peregrine = g_predators[i];

			MtVector3 &v3Position = Peregrine.v3Pos;

			// Render the Peregrine
			myVertex[tri + 0].m_v3Position = v3Position;
			myVertex[tri + 1].m_v3Position = v3Position + (m3Orientation.Col0() * PereguineHalfWingSpan);
			myVertex[tri + 2].m_v3Position = v3Position + (m3Orientation.Col1() * PereguineHalfWingSpan);
			myVertex[tri + 0].m_colour = Peregrine.colour;
			myVertex[tri + 1].m_colour = Peregrine.colour;
			myVertex[tri + 2].m_colour = Peregrine.colour;
			tri += 3;
		}
		for(BtU32 i = 0; i < tri; i += 3)
		{
			myVertex[i + 0].m_v2UV = MtVector2(0, 0);
			myVertex[i + 1].m_v2UV = MtVector2(0, 1);
			myVertex[i + 2].m_v2UV = MtVector2(1, 0);
		}
		m_pBird3->Render(RsPT_TriangleList, pVertex, m_config.NumPredators * 3, MaxSortOrders - 1, BtFalse);
	}

	if( ApConfig::IsDebug() )
	{
		HlDebug::Render();

		MtMatrix4 m4Transform;
		m4Transform.SetTranslation(g_v3Centre);
		HlDraw::RenderCross(m4Transform, m_config.PereguineWingSpan * 2.0f, MaxSortOrders - 1);

        BtChar text[32];
		sprintf(text, "FPS %.0f", RsUtil::GetFPS());
		HlFont::RenderHeavy(MtVector2(100, 15), text, MaxSortOrders - 1);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SbMurmuration::Destroy()
{
}

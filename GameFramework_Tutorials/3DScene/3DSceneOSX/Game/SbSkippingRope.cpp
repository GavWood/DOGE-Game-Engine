////////////////////////////////////////////////////////////////////////////////
// SbSkippingRope.cpp

// Includes
#include "SbSkippingRope.h"
#include "ShTouch.h"
#include "RsUtil.h"
#include "BtTime.h"
#include "HlFont.h"
#include "HlLines.h"
#include "ShIMU.h"
#include "UiKeyboard.h"
#include <stdio.h>

MtVector3 v3P[1024];
MtVector3 v3O[1024];
BtU32 segments = 0;
BtU32 last = 0;
//const float RestLength = 0.06f;
//const float RopeLength = 7.62f;

const float RestLength = 0.03f;
const float RopeLength = 4.57f;

float counter = 0;
MtVector3 v3Start, v3End;
MtVector3 controllerPosition[2];

/*
// http://www.nortechskippingropes.co.uk/jump-ropes/size-guide.aspx
 7ft / 213 cm	up to 4Õ 10Ó / up to 147 cm
 8ft / 244 cm	4Õ 11Ó  to 5Õ 4Ó / 149 cm to 163 cm
 9ft / 274 cm 	5Õ5Ó to 5Õ11Ó / 165 cm to 180 cm
 10ft / 305 cm	6Õ0Ó to 6Õ6Ó /  182cm to 198 cm
 11ft /  335cm	6Õ 7Ó and above / 200cm
 15ft / 457 cm	Our ÔDouble ÐDutch length, enabling at least 2 skipper inside the rope.
 25ft / 762 cm	Ideal for team & class games, fitting many skippers inside the rope.
*/

////////////////////////////////////////////////////////////////////////////////
// Init

void SbSkippingRope::Init()
{
	float z = 0;

    // Init verlet simulation positions
    int index = 0;
    for( float x=0; x<RopeLength; x+= RestLength )
    {
        float usex = x - (RopeLength / 2);
        usex *= 2.0f;
        v3P[index] = MtVector3( usex, 0, z );
        v3O[index] = v3P[index];
        ++index;
    }
    segments = index;
    last = segments - 1;
    v3Start = v3P[0];
    v3End   = v3O[last];
    
    controllerPosition[0] = MtVector3( 0, 0, 0 );
    controllerPosition[1] = MtVector3( 0, 0, 0 );
}

// http://www.gamasutra.com/resource_guide/20030121/jacobson_pfv.htm

////////////////////////////////////////////////////////////////////////////////
// Update

void SbSkippingRope::Update()
{
    static MtVector2 v2CentrePosition;
	for(BtU32 i = 0; i < MaxTouches; i++)
	{
		if(ShTouch::IsPressed(i))
		{
			v2CentrePosition = ShTouch::GetPosition(i);
		}
		else if(ShTouch::IsHeld(i))
		{
			MtVector2 v2Position = ShTouch::GetPosition(i);

			MtVector2 v2Delta = v2CentrePosition - v2Position;

			v2Delta.y /= (BtFloat)RsUtil::GetHeight();
			v2Delta.x /= (BtFloat)RsUtil::GetWidth();
			v3P[0].y    = v2Delta.y;
			v3P[last].y = v2Delta.y;
			v3P[0].z    = v2Delta.x;
			v3P[last].z = v2Delta.x;
		}
	}
    
    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_R))
    {
        Init();
    }
    
    BtU32 numControllers = 2;
    
    for( BtU32 i=0; i<numControllers; i++ )
    {
        MtVector3 v3Accelerometer = ShIMU::GetAccelerometer(i);
        
        // Integrate
        controllerPosition[i].z += v3Accelerometer.x * 0.2f;
        controllerPosition[i].y += v3Accelerometer.y * 0.2f;
        
        // Kill the drift
        controllerPosition[i].x *= 0.9f;
        controllerPosition[i].y *= 0.9f;
        controllerPosition[i].z *= 0.9f;

        if( i == 0 )
        {
            v3P[0].x = v3Start.x + controllerPosition[i].x;
            v3P[0].y = v3Start.y + controllerPosition[i].y;
            v3P[0].z = v3Start.z + controllerPosition[i].z;
            v3O[0]   = v3P[0];
        }
        
        if( ( i == 1 ) || ( numControllers == 1 ) )
        {
            v3P[last].x = v3End.x + controllerPosition[i].x;
            v3P[last].y = v3End.y + controllerPosition[i].y;
            v3P[last].z = v3End.z + controllerPosition[i].z;
            v3O[last]   = v3P[last];
        }
    }
    
	counter += 1.0f / 120.0f;

	// Integrate
	for(BtU32 i = 0; i < segments; i++)
	{
		if((i == 0) || (i == last))
		{
			continue;
		}

		float dt = BtTime::GetTick();
		float dtSq = dt * dt;

		// Derive the velocity
		MtVector3 v3Velocity = v3P[i] - v3O[i];

		v3Velocity *= 0.999f;

		// Add gravity
		MtVector3 v3Acc(0., -9.8f * dtSq, 0);

		// Add the velocity
		MtVector3 v3Next = v3P[i] + v3Velocity + v3Acc;

		v3O[i] = v3P[i];

		v3P[i] = v3Next;

		int a = 0;
		a++;
	}
	int a = 0;
	a++;

	BtU32 itterations = 9;
	for(BtU32 itt = 0; itt < itterations; itt++)
	{
		// Solve the rest lengths
		for(BtU32 i = 0; i < segments - 1; i++)
		{
			int j = i + 1;

			MtVector3 v3Delta = v3P[i] - v3P[j];
			float datalength = v3Delta.GetLength();

			float diff = (datalength - RestLength) / datalength;

			// Move toward solving the constraints
			if(i != 0)
			{
				v3P[i].x -= (v3Delta.x * 0.5f * diff);
				v3P[i].y -= (v3Delta.y * 0.5f * diff);
				v3P[i].z -= (v3Delta.z * 0.5f * diff);
			}

			if(j != last)
			{
				v3P[j].x += (v3Delta.x * 0.5f * diff);
				v3P[j].y += (v3Delta.y * 0.5f * diff);
				v3P[j].z += (v3Delta.z * 0.5f * diff);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbSkippingRope::Render()
{
	BtChar text[32];
	sprintf(text, "%.2f", counter);
	HlFont::Render(MtVector2(0, 20), text, RsColour::BlackColour(), MaxSortOrders - 1);

	for(BtU32 i = 0; i < segments - 2; i += 2)
	{
		MtVector3 v3A = v3P[i];
		MtVector3 v3B = v3P[i + 1];
		HlLines::RenderLine(v3A, v3B, RsColour::WhiteColour(), MaxSortOrders - 1);

		MtVector3 v3C = v3P[i + 1];
		MtVector3 v3D = v3P[i + 2];
		HlLines::RenderLine(v3C, v3D, RsColour::BlackColour(), MaxSortOrders - 1);
		//HlLines::RenderLine( v3A, v3B, RsColour::WhiteColour(), MaxSortOrders - 1);
	}
}

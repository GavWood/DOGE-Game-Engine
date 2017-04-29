////////////////////////////////////////////////////////////////////////////////
// ShIMU

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"

const BtU32 MaxSensors = 16;

struct ShIMUSensor
{
    MtVector3                   m_v3Accelerometer;
    MtVector3                   m_v3Rotation;
    MtQuaternion                m_quaternion;
    MtMatrix4                   m_m4Transform;
};

// Class definition
class ShIMU
{
public:

	// Constructor
	ShIMU();

	// Access

	// State functions
	static void					Update();
    
	// Accessors
    static void                 SetQuaternion( BtU32 index, const MtQuaternion &quaternion );
    static void                 SetAccelerometer( BtU32 index, const MtVector3 &v3Accelerometer );
    static void                 SetRotation( BtU32 index, const MtVector3 &v3Rotation );
    
    static MtMatrix4            GetTransform( BtU32 index );
    static void                 SetTransform( BtU32 index, MtMatrix4 &m4Transform );
    static MtVector3            GetAccelerometer( BtU32 index );
    static MtQuaternion         GetQuaternion( BtU32 index );
    static BtU32                GetNumSensors();
    static void                 SetNumSensors( BtU32 sensors );
    
private:

	// Private members
    static ShIMUSensor          m_sensors[MaxSensors];
    static BtU32                m_numSensors;
};

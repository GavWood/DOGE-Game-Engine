////////////////////////////////////////////////////////////////////////////////
// ShIMU

// Includes
#include "ShIMU.h"
#include "RsUtil.h"
#include "BtMemory.h"
#include "UiKeyboard.h"

// Statics
ShIMUSensor ShIMU::m_sensors[MaxSensors];
BtU32 ShIMU::m_numSensors = 0;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ShIMU::ShIMU()
{
    ShIMUSensor sensor;
    sensor.m_quaternion.SetIdentity();
    sensor.m_v3Accelerometer = MtVector3( 0, 0, 0 );
    sensor.m_m4Transform.SetIdentity();
    
    for( BtU32 i=0; i<MaxSensors; i++ )
    {
        m_sensors[i] = sensor;
    }
    
    m_numSensors = 0;
}

///////////////////////////////////////////////////////////////////////////////
// GetNumSensors

BtU32 ShIMU::GetNumSensors()
{
    return m_numSensors;
}

///////////////////////////////////////////////////////////////////////////////
// SetNumSensors

void ShIMU::SetNumSensors( BtU32 sensors )
{
    m_numSensors = sensors;
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShIMU::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ShIMU::SetQuaternion( BtU32 index, const MtQuaternion &quaternion )
{
    m_sensors[index].m_m4Transform = quaternion;
    m_sensors[index].m_quaternion = quaternion;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ShIMU::SetAccelerometer( BtU32 index, const MtVector3 &v3Accelerometer )
{
    m_sensors[index].m_v3Accelerometer = v3Accelerometer;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ShIMU::SetRotation( BtU32 index, const MtVector3 &v3Rotation )
{
    m_sensors[index].m_v3Rotation = v3Rotation;
}

////////////////////////////////////////////////////////////////////////////////
// Update

MtMatrix4 ShIMU::GetTransform( BtU32 index )
{
    return m_sensors[index].m_m4Transform;
}

void ShIMU::SetTransform( BtU32 index, MtMatrix4 &m4Transform )
{
    m_sensors[index].m_m4Transform = m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// Update

MtVector3 ShIMU::GetAccelerometer( BtU32 index )
{
    return m_sensors[index].m_v3Accelerometer;
}

////////////////////////////////////////////////////////////////////////////////
// Update

MtQuaternion ShIMU::GetQuaternion( BtU32 index )
{
    return m_sensors[index].m_quaternion;
}

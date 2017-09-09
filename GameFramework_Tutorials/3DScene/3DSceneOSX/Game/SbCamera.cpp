////////////////////////////////////////////////////////////////////////////////
// ShCamera.cpp

#include "ApConfig.h"
#include "BtTime.h"
#include "MtMatrix3.h"
#include "SbCamera.h"
#include "ShTouch.h"
#include "UiKeyboard.h"
#include "RsUtil.h"
#include "FsFile.h"
#include "HlKeyboard.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Init

void ShCamera::Init()
{
	BtFloat width = (BtFloat)RsUtil::GetWidth();
	BtFloat height = (BtFloat)RsUtil::GetHeight();

	m_cameraData.m_camera = RsCamera( 0.1f, 10000.0f, width / height, RsViewport( 0, 0, (BtU32)width, (BtU32)height ), MtDegreesToRadians( 60.0f ) );
	m_cameraData.m_camera.SetDimension( RsUtil::GetDimension() );
	m_cameraData.m_camera.SetPerspective( BtTrue );

	m_cameraData.m_v3Position = MtVector3( 0, 100, 0 );
	m_cameraData.m_m3Rotation.SetIdentity();

	m_cameraData.m_speed = 100.0f;
	m_cameraData.m_pitch = 0;
	m_cameraData.m_yaw = 0;
}

////////////////////////////////////////////////////////////////////////////////
// SetDimension

void ShCamera::SetDimension( const MtVector2 &v2Dimension )
{
	BtFloat width = v2Dimension.x;
	BtFloat height = v2Dimension.y;

	BtFloat fieldOfView = m_cameraData.m_camera.FieldOfView();
	BtFloat nearPlane   = m_cameraData.m_camera.NearPlane();
	BtFloat farPlane    = m_cameraData.m_camera.FarPlane();
	m_cameraData.m_camera = RsCamera( nearPlane, farPlane, width / height, RsViewport( 0, 0, (BtU32)width, (BtU32)height ), fieldOfView );
	m_cameraData.m_camera.SetDimension( v2Dimension );
	m_cameraData.m_camera.SetPerspective( BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// SetPosition

void ShCamera::SetPosition( const MtVector3 &v3Position )
{
	m_cameraData.m_v3Position = v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

MtVector3 ShCamera::GetPosition()
{
	return m_cameraData.m_v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// SetSpeed

void ShCamera::SetSpeed( BtFloat speed )
{
	m_cameraData.m_speed = speed;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ShCamera::Update()
{   
	static BtBool isLoaded = BtTrue;
	if( isLoaded == BtFalse )
	{
		// If we have a previously saved version of the camera then load
		FsFile file;
		BtChar filename[64];
		sprintf(filename, "%s%s", ApConfig::GetDocuments(), "camera.txt");
		file.Open( filename, eFsMode_Read );
		if(file.IsOpen())
		{
			file.Read(m_cameraData);
			file.Close();
		}
		isLoaded = BtTrue;
	}

	BtFloat speed = BtTime::GetTick() * 100.0f;

	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_LSHIFT ) )
	{
		speed = speed * 10.0f;
	}
	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_W ) )
	{
		MoveForward( speed );
	}
	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_S ) )
	{
		MoveBackward( speed );
	}
	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_D ) )
	{
		MoveRight( speed );
	}
	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_A ) )
	{
		MoveLeft( speed );
	}
	
	speed = BtTime::GetTick() * 0.1f;

	for( BtU32 i=1; i<MaxTouches; i+=2 )
	{
		if( ShTouch::IsHeld(i) )
		{
			MtVector2 v2MouseDirection = ShTouch::GetMovement(i);

			// Rotate the camera
			MtMatrix3 m3Rotate;
			m3Rotate.SetRotationY( v2MouseDirection.x * -speed );
			m_cameraData.m_m3Rotation = m3Rotate * m_cameraData.m_m3Rotation;
			m3Rotate.SetRotationX( v2MouseDirection.y * speed );
			m_cameraData.m_m3Rotation = m_cameraData.m_m3Rotation * m3Rotate;
		}
	}

	// Rotate the camera
	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_LEFT ) )
	{
		m_cameraData.m_yaw += speed * 10.0f;
	}
	if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_RIGHT ) )
	{
		m_cameraData.m_yaw -= speed * 10.0f;
	}
	if(UiKeyboard::pInstance()->IsHeld(UiKeyCode_UP))
	{
		m_cameraData.m_pitch -= speed * 10.0f;
	}
	if(UiKeyboard::pInstance()->IsHeld(UiKeyCode_DOWN))
	{
		m_cameraData.m_pitch += speed * 10.0f;
	}
	MtMatrix3 m3RotateY;
	m3RotateY.SetRotationX( m_cameraData.m_pitch );
	MtMatrix3 m3RotateX;
	m3RotateX.SetRotationY( m_cameraData.m_yaw );
	m_cameraData.m_m3Rotation = m3RotateX * m3RotateY;

	// Cache the ShHMD rotation
	//MtQuaternion quaternion = ShHMD::GetQuaternion();
	//quaternion.SetIdentity();

	// Set the IMU rotation
    MtMatrix4 m4FinalRotation = m_cameraData.m_m3Rotation;// * MtMatrix3( quaternion );

	// Set the position
	m_cameraData.m_camera.SetPosition( m_cameraData.m_v3Position );

	// Set the rotation
	m_cameraData.m_camera.SetRotation( m4FinalRotation );

	// Update the camera
	m_cameraData.m_camera.Update();

	if(UiKeyboard::pInstance()->IsPressed(SaveCameraKey) )
	{
		FsFile file;
		BtChar filename[64];
		sprintf( filename, "%s%s", ApConfig::GetDocuments(), "camera.txt" );
		file.Open( filename, eFsMode_Write );
		if( file.IsOpen() )
		{
			file.Write( m_cameraData );
			file.Close();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// MoveUpward

void ShCamera::MoveRight( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;

	MtMatrix3 m3Rotation = m_cameraData.m_camera.GetRotation();
	m_cameraData.m_v3Position += m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// MoveDownward

void ShCamera::MoveLeft( BtFloat speed )
{
	speed = BtTime::GetTick() * -speed;

	MtMatrix3 m3Rotation = m_cameraData.m_camera.GetRotation();
	m_cameraData.m_v3Position += m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// MoveForward

void ShCamera::MoveForward( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;

	MtMatrix3 m3Rotation = m_cameraData.m_camera.GetRotation();
	m_cameraData.m_v3Position += m3Rotation.Col2() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// MoveBackward

void ShCamera::MoveBackward( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;

	MtMatrix3 m3Rotation = m_cameraData.m_camera.GetRotation();
	 m_cameraData.m_v3Position -= m3Rotation.Col2() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// StrafeRight

void ShCamera::StrafeRight( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;
	MtMatrix3 m3Rotation =  m_cameraData.m_camera.GetRotation();
	 m_cameraData.m_v3Position += m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// StrafeLeft

void ShCamera::StrafeLeft( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;
	MtMatrix3 m3Rotation =  m_cameraData.m_camera.GetRotation();
	m_cameraData.m_v3Position -= m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// GetCamera

RsCamera &ShCamera::GetCamera()
{
	return m_cameraData.m_camera;
}

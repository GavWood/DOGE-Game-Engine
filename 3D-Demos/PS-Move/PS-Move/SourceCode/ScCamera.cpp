////////////////////////////////////////////////////////////////////////////////
// SbCamera.cpp

#include "ApConfig.h"
#include "BtTime.h"
#include "MtMatrix3.h"
#include "ShTouch.h"
#include "UiKeyboard.h"
#include "RsUtil.h"
#include "ShHMD.h"
#include "FsFile.h"
#include "HlKeyboard.h"
#include "HlModel.h"
#include "ShIMU.h"
#include <stdio.h>

#include "ScMain.h"
#include "ScCamera.h"

////////////////////////////////////////////////////////////////////////////////
// Init

void SbCamera::Init( MtVector2 v2Dimension )
{
	BtFloat width  = v2Dimension.x;
	BtFloat height = v2Dimension.y;

	m_camera = RsCamera( 0.01f, 1000.0f, width / height, RsViewport( 0, 0, (BtU32)width, (BtU32)height ), MtDegreesToRadians( 60.0f ) );
	m_camera.SetDimension(MtVector2(width, height));
	m_camera.SetPerspective( BtTrue );

	m_cameraData.m_v3Position = MtVector3( 0, 0, -2.0f );
	m_cameraData.m_m3Rotation.SetIdentity();
    
    MtMatrix3 m3Rotation;
    m3Rotation.LookAt( m_cameraData.m_v3Position, MtVector3( 0, 0, 0 ), MtVector3( 0, 1, 0 ) );
    m_cameraData.m_m3Rotation = m3Rotation;
    
	m_speed = 100.0f;
	m_cameraData.m_pitch = 0;
	m_cameraData.m_yaw = 0;
}

////////////////////////////////////////////////////////////////////////////////
// SetPosition

void SbCamera::SetPosition( const MtVector3 &v3Position )
{
	m_cameraData.m_v3Position = v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

MtVector3 SbCamera::GetPosition()
{
	return m_cameraData.m_v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// SetSpeed

void SbCamera::SetSpeed( BtFloat speed )
{
	m_speed = speed;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbCamera::Update()
{
    static MtMatrix3 m3StartingRotation;

    static BtBool isLoaded = BtFalse;
    if( isLoaded == BtFalse )
    {
        // Load the camera
        FsFile file;
        BtChar filename[256];
        sprintf(filename, "%s%s", ApConfig::GetResourcePath(), "camera.txt");
        file.Open( filename, FsMode_Read );
        if(file.IsOpen())
        {
            file.Read(m_cameraData);
            file.Close();
        }
        isLoaded = BtTrue;
        
        // Set the starting rotation
        m3StartingRotation = m_cameraData.m_m3Rotation;
    }

    BtFloat speed = BtTime::GetTick() * m_speed;
    
    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LSHIFT))
    {
        speed = speed * 10.0f;
    }
    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_W))
    {
        MoveForward(speed);
    }
    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_S))
    {
        MoveBackward(speed);
    }
    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_D))
    {
        MoveRight(speed);
    }
    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_A))
    {
        MoveLeft(speed);
    }

    // Rotate the camera
    speed = BtTime::GetTick();

    if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LSHIFT))
    {
        if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LEFT))
        {
            m_cameraData.m_yaw += speed;
        }
        if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_RIGHT))
        {
            m_cameraData.m_yaw -= speed;
        }
        if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_UP))
        {
            m_cameraData.m_pitch -= speed;
        }
        if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_DOWN))
        {
            m_cameraData.m_pitch += speed;
        }
    }

    MtMatrix3 m3RotateY;
    m3RotateY.SetRotationX(m_cameraData.m_pitch);
    MtMatrix3 m3RotateX;
    m3RotateX.SetRotationY(m_cameraData.m_yaw);

    m_cameraData.m_m3Rotation = m3StartingRotation * m3RotateX * m3RotateY;

    if( UiKeyboard::pInstance()->IsPressed(SaveCameraKey) )
    {
        FsFile file;
        BtChar filename[256];
        sprintf( filename, "%s%s", ApConfig::GetResourcePath(), "camera.txt" );
        file.Open( filename, FsMode_Write );
        if( file.IsOpen() )
        {
            file.Write( m_cameraData );
            file.Close();
        }
    }
}

void SbCamera::Render()
{
    // Set the rotation
    m_camera.SetRotation(m_cameraData.m_m3Rotation);
        
    // Set the position
    m_camera.SetPosition(m_cameraData.m_v3Position);

	// Update the camera
	m_camera.Update();
}

////////////////////////////////////////////////////////////////////////////////
// SetFarPlane

void SbCamera::SetFarPlane(BtFloat farPlane )
{
	m_camera.FarPlane( farPlane );
}

////////////////////////////////////////////////////////////////////////////////
// MoveUpward

void SbCamera::MoveRight( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;

	MtMatrix3 m3Rotation = m_camera.GetRotation();
	m_cameraData.m_v3Position += m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// MoveDownward

void SbCamera::MoveLeft( BtFloat speed )
{
	speed = BtTime::GetTick() * -speed;

	MtMatrix3 m3Rotation = m_camera.GetRotation();
	m_cameraData.m_v3Position += m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// MoveForward

void SbCamera::MoveForward( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;

	MtMatrix3 m3Rotation = m_camera.GetRotation();
	m_cameraData.m_v3Position += m3Rotation.Col2() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// MoveBackward

void SbCamera::MoveBackward( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;

	MtMatrix3 m3Rotation = m_camera.GetRotation();
	m_cameraData.m_v3Position -= m3Rotation.Col2() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// StrafeRight

void SbCamera::StrafeRight( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;
	MtMatrix3 m3Rotation =  m_camera.GetRotation();
    m_cameraData.m_v3Position += m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// StrafeLeft

void SbCamera::StrafeLeft( BtFloat speed )
{
	speed = BtTime::GetTick() * speed;
	MtMatrix3 m3Rotation =  m_camera.GetRotation();
	m_cameraData.m_v3Position -= m3Rotation.Col0() * speed;
}

////////////////////////////////////////////////////////////////////////////////
// GetCamera

RsCamera &SbCamera::GetCamera()
{
	return m_camera;
}

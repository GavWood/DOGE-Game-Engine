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

	m_camera = RsCamera( 0.1f, 1000.0f, width / height, RsViewport( 0, 0, (BtU32)width, (BtU32)height ), MtDegreesToRadians( 60.0f ) );
	m_camera.SetDimension(MtVector2(width, height));
	m_camera.SetPerspective( BtTrue );

	m_cameraData.m_v3Position = MtVector3( 1.5f, 0, 2.0f );
	m_cameraData.m_m3Rotation.SetIdentity();
    
    MtMatrix3 m3Rotation;
    m3Rotation.LookAt( m_cameraData.m_v3Position, MtVector3( 0, 0, 0 ), MtVector3( 0, 1, 0 ) );
    m_cameraData.m_m3Rotation = m3Rotation;
    
	m_speed = 100.0f;
	m_cameraData.m_pitch = 0;
	m_cameraData.m_yaw = 0;
  //  m_cameraData.m_v3Position = MtVector3( 0, 0, 4.0f );
	m_isFlyCam = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// SetDimension

void SbCamera::SetDimension( const MtVector2 &v2Dimension )
{
	BtFloat width = v2Dimension.x;
	BtFloat height = v2Dimension.y;

	BtFloat fieldOfView = m_camera.FieldOfView();
	BtFloat nearPlane   = m_camera.NearPlane();
	BtFloat farPlane    = m_camera.FarPlane();
	m_camera = RsCamera( nearPlane, farPlane, width / height, RsViewport( 0, 0, (BtU32)width, (BtU32)height ), fieldOfView );
	m_camera.SetDimension( v2Dimension );
	m_camera.SetPerspective( BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// IsFlyCam

BtBool SbCamera::IsFlyCam()
{
	return m_isFlyCam;
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
    if( ApConfig::IsPhone() )
    {
        if (ShTouch::IsHeld(1))
        {
            BtBool onScreen = ShTouch::OnScreen(1);
            if( onScreen )
            {
                for (BtU32 i = 0; i < MaxTouches; i += 2)
                {
                    // Rotate the camera
                    MtVector2 v2MouseDirection = ShTouch::GetMovement(i);
                    BtFloat speed = BtTime::GetTick() * 0.1f;
                    MtMatrix3 m3Rotate;
                    m3Rotate.SetRotationY(v2MouseDirection.x * -speed);
                    m_cameraData.m_m3Rotation = m3Rotate * m_cameraData.m_m3Rotation;
                    m3Rotate.SetRotationX(v2MouseDirection.y * speed);
                    m_cameraData.m_m3Rotation = m_cameraData.m_m3Rotation * m3Rotate;
                }
            }
        }
    }
    else if( ApConfig::IsDesktop() )
    {
        static BtBool isLoaded = BtFalse;
        if( isLoaded == BtFalse )
        {
            // If we have a previously saved version of the camera then load
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
        }
        if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_F3))
        {
            m_isFlyCam = !m_isFlyCam;
        }
        for (BtU32 i = 1; i < MaxTouches; i += 2)
        {
            if (ShTouch::IsHeld(i) || m_isFlyCam)
            {
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
            }
        }

        static BtBool isCursorKeys = BtFalse;
        static MtMatrix3 m3StartingRotation;

        // Rotate the camera
        BtFloat speed = BtTime::GetTick();

        BtBool oldCursorKeysState = isCursorKeys;
        
        if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LSHIFT))
        {
            if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_LEFT))
            {
                isCursorKeys = BtTrue;
                m_cameraData.m_yaw += speed;
            }
            if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_RIGHT))
            {
                isCursorKeys = BtTrue;
                m_cameraData.m_yaw -= speed;
            }
            if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_UP))
            {
                isCursorKeys = BtTrue;
                m_cameraData.m_pitch -= speed;
            }
            if (UiKeyboard::pInstance()->IsHeld(UiKeyCode_DOWN))
            {
                isCursorKeys = BtTrue;
                m_cameraData.m_pitch += speed;
            }
        }
        if (isCursorKeys)
        {
            MtMatrix3 m3RotateY;
            m3RotateY.SetRotationX(m_cameraData.m_pitch);
            MtMatrix3 m3RotateX;
            m3RotateX.SetRotationY(m_cameraData.m_yaw);
            
            if( oldCursorKeysState != isCursorKeys )
            {
				//m3StartingRotation = m_cameraData.m_m3Rotation;
            }
            m_cameraData.m_m3Rotation = m3RotateX * m3RotateY;
        }
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
}

void SbCamera::Render()
{
    if( ApConfig::IsDesktop() )
    {
        // Set the rotation
        m_camera.SetRotation(m_cameraData.m_m3Rotation);
        
        // Set the position
        m_camera.SetPosition(m_cameraData.m_v3Position);
    }
    else // IsPhone()
	{
        if( ApConfig::IsAR()  )
        {
            // Support a landscape quaternion
            MtMatrix3 m_m3Rotation;
            MtQuaternion quaternion = ShIMU::GetQuaternion(0);
            quaternion.x =  quaternion.x;
            quaternion.y =  quaternion.y;
            quaternion.z = -quaternion.z;
            m_m3Rotation = MtMatrix3(quaternion);
            m_camera.SetRotation(m_m3Rotation);

            // Set the position
            MtVector3 v3Position = ShIMU::GetPosition(0); // m_cameraData.m_v3Position
            m_camera.SetPosition( m_cameraData.m_v3Position + v3Position );
        }
        else if( ShHMD::IsHMD() )
        {
            MtMatrix4 m4Projection = ShHMD::GetProjectionMatrix();
            m_camera.SetProjection(m4Projection);

            MtMatrix3 m3RotateY;
            m3RotateY.SetRotationX(m_cameraData.m_pitch);
            MtMatrix3 m3RotateX;
            m3RotateX.SetRotationY(m_cameraData.m_yaw);
            m_cameraData.m_m3Rotation = m3RotateX * m3RotateY;

            // Cache the ShHMD rotation
            MtQuaternion quaternion = ShHMD::GetQuaternion();

            // Set the IMU rotation
            MtMatrix4 m4FinalRotation = MtMatrix3(quaternion);

            // Set the rotation
            m_camera.SetRotation(m4FinalRotation);

            MtVector3 v3HMDPosition = ShHMD::GetPosition();

            // Set the position
            m_camera.SetPosition(m_cameraData.m_v3Position + v3HMDPosition );
        }
        else
        {
            // Set the rotation
            m_camera.SetRotation(m_cameraData.m_m3Rotation);
            
            // Set the position
            m_camera.SetPosition(m_cameraData.m_v3Position);
        }
    }

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

////////////////////////////////////////////////////////////////////////////////
// SbCamera.h

// Include guard
#pragma once
#include "BtBase.h"
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsCamera.h"
#include "MtMatrix3.h"

struct ShCameraData
{
	MtVector3					m_v3Position;
	MtMatrix3					m_m3Rotation;
	BtFloat						m_pitch;
	BtFloat						m_yaw;
};

// Class definition
class SbCamera
{
public:

	// Public functions
	void						Init( MtVector2 v2Dimension );
	void						Update();
	void						Render();
	
	void						StrafeLeft( BtFloat speed );
	void						StrafeRight( BtFloat speed );
	void						MoveRight( BtFloat speed );
	void						MoveLeft( BtFloat speed );
	void						MoveForward( BtFloat speed );
	void						MoveBackward( BtFloat speed );
	void						UpdateRotation( const MtVector2 &v2Movement );
	
	// Accessors
	RsCamera				   &GetCamera();
	void						SetPosition( const MtVector3 &v3Position );
	MtVector3					GetPosition();
	void						SetSpeed( BtFloat speed );
	void						SetFarPlane( BtFloat );

private:

	// Private members
    
	ShCameraData				m_cameraData;
	RsCamera					m_camera;
	BtFloat						m_speed;
};

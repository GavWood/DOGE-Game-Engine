////////////////////////////////////////////////////////////////////////////////
// ShHMD.h

// Include guard
#pragma once
#include "BtBase.h"
#include "MtMatrix4.h"
#include "MtVector2.h"

class RsTexture;
class RsMaterial;

// Class definition
class ShHMD
{
public:

	// Accessors
	static void                 SetHMD( BtBool isEnabled );
	static BtBool				IsHMD();

	static BtBool				IsMirror();
	static void					SetMirror(BtBool);

	static void					SetDimension( MtVector2 v2Dimension );
	static MtVector2			GetDimension();
	
	static RsMaterial		   *GetMaterial( BtU32 index );
	static void				    SetMaterial( BtU32 index, RsMaterial *pMaterial );

	static void					SetProjectionMatrix( const MtMatrix4 &m4Projection );
	static MtMatrix4		    GetProjectionMatrix();

	static void                 SetQuaternion( MtQuaternion &quaternion );
	static MtQuaternion         GetQuaternion();

	static void                 SetPosition(MtVector3 &v3Position);
	static MtVector3            GetPosition();

	static void                 SetNearPlane( BtFloat nearPlane );
	static BtFloat	            GetNearPlane();

	static void                 SetFarPlane( BtFloat farPlane );
	static BtFloat	            GetFarPlane();

	static MtMatrix3            GetRotation();

private:

	static BtFloat				m_farPlane;
	static BtFloat				m_nearPlane;

	static BtBool				m_isHMD;
	static BtBool				m_isMirror;
	static RsMaterial		   *m_materials[2];
	static MtMatrix4			m_projection;
	static MtVector3			m_v3Position;
	static MtQuaternion			m_quaternion;
	static MtMatrix3			m_m3Rotation;
	static MtVector2            m_v2Dimension;
};

////////////////////////////////////////////////////////////////////////////////
// SgCameraDX11.h

#pragma once
#include "SgCamera.h"
#include "BaFileData.h"
#include "ErrorLog.h"

// Class Declaration
class SgCameraDX11 : public SgCamera
{
public:

	void					FixPointers( BtU8 *pFileData );

private:

	friend class SgNodeDX11;
	friend class SgNodeWinGL;

	BtFloat 				FarPlane() const;
	BtFloat 				NearPlane() const;
	BtFloat 				FieldOfView() const;

	SgNode				   *m_pNode;
	BaSgCameraFileData	   *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// FixPointers

inline void SgCameraDX11::FixPointers( BtU8 *pFileData )
{
	// Set the file data
	m_pFileData = (BaSgCameraFileData*) pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// FarPlane

inline BtFloat SgCameraDX11::FarPlane() const
{
	return m_pFileData->m_fFarPlane;
}

////////////////////////////////////////////////////////////////////////////////
// NearPlane

inline BtFloat SgCameraDX11::NearPlane() const
{
	return m_pFileData->m_fNearPlane;
}

////////////////////////////////////////////////////////////////////////////////
// FieldOfView

inline BtFloat SgCameraDX11::FieldOfView() const
{
	return m_pFileData->m_fFieldOfView;
}

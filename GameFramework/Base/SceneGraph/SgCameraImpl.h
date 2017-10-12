////////////////////////////////////////////////////////////////////////////////
// SgCameraImpl.h

#pragma once
#include "SgCamera.h"
#include "BaFileData.h"
#include "ErrorLog.h"

// Class Declaration
class SgCameraImpl : public SgCamera
{
public:

	void					FixPointers( BtU8 *pFileData );

private:

	friend class SgNodeDX11;
	friend class SgNodeImpl;

	BtFloat 				FarPlane() const;
	BtFloat 				NearPlane() const;
	BtFloat 				FieldOfView() const;

	SgNode				   *m_pNode;
	BaSgCameraFileData	   *m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// FixPointers

inline void SgCameraImpl::FixPointers( BtU8 *pFileData )
{
	// Set the file data
	m_pFileData = (BaSgCameraFileData*) pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// FarPlane

inline BtFloat SgCameraImpl::FarPlane() const
{
	return m_pFileData->m_fFarPlane;
}

////////////////////////////////////////////////////////////////////////////////
// NearPlane

inline BtFloat SgCameraImpl::NearPlane() const
{
	return m_pFileData->m_fNearPlane;
}

////////////////////////////////////////////////////////////////////////////////
// FieldOfView

inline BtFloat SgCameraImpl::FieldOfView() const
{
	return m_pFileData->m_fFieldOfView;
}

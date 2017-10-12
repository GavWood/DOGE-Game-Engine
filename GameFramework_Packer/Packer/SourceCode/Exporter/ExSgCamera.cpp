///////////////////////////////////////////////////////////////////////////////
// ExSgCamera.cpp

// Includes
#include "StdAfx.h"
#include "FCollada.h"
#include "ExMatrix.h"
#include "ExSgCamera.h"
#include "ExScene.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgCamera::ExSgCamera( ExSgNode *pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
	m_fNearPlane = 0.1f;
	m_fFarPlane = 1000.0f;
	m_fFieldOfView = MtPI / 4.0f;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgCamera::~ExSgCamera()
{
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgCamera::ChangeCoordinateSystem()
{

}

///////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgCamera::CopyAttributes()
{
	m_pNode->m_cameraFileData.m_fNearPlane = m_fNearPlane;
	m_pNode->m_cameraFileData.m_fFarPlane = m_fFarPlane;
	m_pNode->m_cameraFileData.m_fFieldOfView = m_fFieldOfView;
}

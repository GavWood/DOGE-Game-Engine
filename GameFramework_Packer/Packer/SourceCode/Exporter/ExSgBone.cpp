////////////////////////////////////////////////////////////////////////////////
// ExSgBone.cpp

// Includes
#include "StdAfx.h"
#include "FCollada.h"
#include "ExSgBone.h"
#include "ExSgNode.h"
#include "ExMatrix.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgBone::ExSgBone( ExSgNode *pNode, ExScene *pScene, BtU32 iJoint )
{
	m_pNode = pNode;
	m_pScene = pScene;
	m_iJoint = iJoint;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgBone::~ExSgBone()
{
}

///////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgBone::CopyAttributes()
{
	m_pNode->m_boneFileData.m_m4InverseBindPose = m_pNode->m_pBone->InverseBindPose();
	// m_pNode->m_boneFileData.m_m4InverseBindPose = m_pNode->World().Inverse();
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgBone::ChangeCoordinateSystem()
{
	ExMatrix::ChangeCoordinateSystem( m_m4InverseBindPose );
}

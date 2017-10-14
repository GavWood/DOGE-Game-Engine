///////////////////////////////////////////////////////////////////////////////
// ExSgLight.cpp

// Includes
#include "StdAfx.h"
#include "ExMatrix.h"
#include "ExSgLight.h"
#include "ExScene.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgLight::ExSgLight( ExSgNode *pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgLight::~ExSgLight()
{
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgLight::ChangeCoordinateSystem()
{

}

///////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgLight::CopyAttributes()
{
	m_pNode->m_lightFileData.m_intensity = m_intensity;
	m_pNode->m_lightFileData.m_type = m_type;
	m_pNode->m_lightFileData.m_colour = m_colour;
	m_pNode->m_lightFileData.m_isLit = BtTrue;
	m_pNode->m_lightFileData.m_fallOffAngle = m_fallOffAngle;
	m_pNode->m_lightFileData.m_outerAngle = m_outerAngle;
}

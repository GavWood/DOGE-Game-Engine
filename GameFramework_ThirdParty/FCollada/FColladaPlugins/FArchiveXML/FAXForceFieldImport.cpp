/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FArchiveXML.h"
#include "FCDAnimated.h"
#include "FCDExtra.h"
#include "FCDForceTyped.h"
#include "FCDForceDeflector.h"
#include "FCDForceDrag.h"
#include "FCDForceGravity.h"
#include "FCDForcePBomb.h"
#include "FCDForceWind.h"
#include "FCDForceField.h"

				
bool FArchiveXML::LoadForceField(FCDObject* object, xmlNode* node)					
{
	FCDForceField* forceField = (FCDForceField*)object;

	bool status = FArchiveXML::LoadEntity(object, node);
	if (!status) return status;
	if (!IsEquivalent(node->name, DAE_FORCE_FIELD_ELEMENT))
	{
		FUError::Error(FUError::WARNING_LEVEL, FUError::WARNING_UNKNOWN_FORCE_FIELD_ELEMENT, node->line);
		return status;
	}


	status &= (FArchiveXML::LoadExtra(forceField->GetInformation(), node));
	
	forceField->SetDirtyFlag();
	return status;
}

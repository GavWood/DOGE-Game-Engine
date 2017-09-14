/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FArchiveXML.h"
#include "FCDEmitter.h"
#include "FCDEmitterObject.h"
#include "FCDEmitterParticle.h"
#include "FCDParticleModifier.h"


//
// Emitter Import
//

bool FArchiveXML::LoadEmitter(FCDObject* object, xmlNode* node)				
{
	FCDEmitter* emitter = (FCDEmitter*)object;

	bool status = FArchiveXML::LoadEntity(emitter, node);
	if (!status) return status;
	if (!IsEquivalent(node->name, DAE_EMITTER_ELEMENT))
	{
		FUError::Error(FUError::WARNING_LEVEL, FUError::WARNING_UNKNOWN_ELEMENT, node->line);
		return status;
	}


	emitter->SetDirtyFlag();
	return status;
}


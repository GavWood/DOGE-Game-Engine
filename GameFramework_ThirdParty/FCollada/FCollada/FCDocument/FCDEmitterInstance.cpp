/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FCDocument.h"
#include "FCDEmitter.h"
#include "FCDEmitterInstance.h"
#include "FCDEntity.h"
#include "FCDExtra.h"
#include "FCDGeometryPolygons.h"
#include "FCDLibrary.h"
#include "FCDMaterial.h"
#include "FCDMaterialInstance.h"

//
// FCDEmitterInstance
//

ImplementObjectType(FCDEmitterInstance);
ImplementParameterObjectNoCtr(FCDEmitterInstance, FCDEntityInstance, forceInstances);
ImplementParameterObjectNoCtr(FCDEmitterInstance, FCDEntityInstance, emittedInstances);

FCDEmitterInstance::FCDEmitterInstance(FCDocument* document, FCDSceneNode* parent, FCDEntity::Type entityType)
:	FCDEntityInstance(document, parent, entityType)
{
}

FCDEmitterInstance::~FCDEmitterInstance()
{
}


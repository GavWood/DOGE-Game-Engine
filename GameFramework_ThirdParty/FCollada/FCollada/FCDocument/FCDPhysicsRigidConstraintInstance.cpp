/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FCDocument.h"
#include "FCDEntity.h"
#include "FCDPhysicsModel.h"
#include "FCDPhysicsModelInstance.h"
#include "FCDPhysicsRigidBody.h"
#include "FCDPhysicsRigidConstraint.h"
#include "FCDPhysicsRigidConstraintInstance.h"
#include "FUUri.h"

ImplementObjectType(FCDPhysicsRigidConstraintInstance);

FCDPhysicsRigidConstraintInstance::FCDPhysicsRigidConstraintInstance(FCDocument* document, FCDPhysicsModelInstance* _parent, FCDPhysicsRigidConstraint* constraint)
:	FCDEntityInstance(document, NULL, FCDEntity::PHYSICS_RIGID_CONSTRAINT), parent(_parent)
{
	if (constraint != NULL)
	{
		SetRigidConstraint(constraint); 
	}
}

FCDPhysicsRigidConstraintInstance::~FCDPhysicsRigidConstraintInstance()
{
	parent = NULL;
}

FCDEntityInstance* FCDPhysicsRigidConstraintInstance::Clone(FCDEntityInstance* _clone) const
{
	FCDPhysicsRigidConstraintInstance* clone = NULL;
	if (_clone == NULL) _clone = clone = new FCDPhysicsRigidConstraintInstance(const_cast<FCDocument*>(GetDocument()), NULL, NULL);
	else if (_clone->HasType(FCDPhysicsRigidConstraintInstance::GetClassType())) clone = (FCDPhysicsRigidConstraintInstance*) _clone;

	Parent::Clone(_clone);

	if (clone != NULL)
	{
		// No interesting data to clone.
	}

	return _clone;
}

void FCDPhysicsRigidConstraintInstance::SetRigidConstraint(
		FCDPhysicsRigidConstraint* constraint)
{
	FUAssert(constraint != NULL, ;);

	SetEntity(constraint); 
}

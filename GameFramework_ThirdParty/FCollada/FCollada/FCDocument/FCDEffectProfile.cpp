/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FCDocument.h"
#include "FCDEffect.h"
#include "FCDEffectProfile.h"
#include "FCDEffectParameter.h"
#include "FCDEffectParameterFactory.h"
#include "FCDExtra.h"
#include "FCDImage.h"
#include "FCDLibrary.h"

//
// FCDEffectProfile
//

ImplementObjectType(FCDEffectProfile);
ImplementParameterObjectNoCtr(FCDEffectProfile, FCDEffectParameter, parameters);
ImplementParameterObject(FCDEffectProfile, FCDExtra, extra, new FCDExtra(parent->GetDocument(), parent));

FCDEffectProfile::FCDEffectProfile(FCDocument* document, FCDEffect* _parent)
:	FCDObject(document), parent(_parent)
,	InitializeParameterNoArg(parameters)
,	InitializeParameterNoArg(extra)
{
	extra = new FCDExtra(GetDocument(), this);
}

FCDEffectProfile::~FCDEffectProfile()
{
	parent = NULL;
}

FCDEffectProfile* FCDEffectProfile::Clone(FCDEffectProfile* clone) const
{
	if (clone == NULL) return NULL;
	size_t parameterCount = parameters.size();
	for (size_t p = 0; p < parameterCount; ++p)
	{
		FCDEffectParameter* parameter = clone->AddEffectParameter(parameters[p]->GetType());
		parameters[p]->Clone(parameter);
	}
	extra->Clone(clone->extra);
	return clone;
}

FCDEffectParameter* FCDEffectProfile::AddEffectParameter(uint32 type)
{
	FCDEffectParameter* parameter = FCDEffectParameterFactory::Create(GetDocument(), type);
	parameters.push_back(parameter);
	SetNewChildFlag();
	return parameter;
}

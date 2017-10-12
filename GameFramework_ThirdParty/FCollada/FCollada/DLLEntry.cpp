/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"

#ifdef FCOLLADA_DLL
#ifdef WIN32
HINSTANCE hInstance = NULL;

BOOL WINAPI DllMain(HINSTANCE _hInstance, ULONG fdwReason, LPVOID UNUSED(lpvReserved))
{
	static int initCount = 0;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		// Initialize and Release are ref-counted.
		if (initCount == 0)
		{
			//FCollada::Initialize();
		}
		initCount++;
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_DETACH:
		// Initialize and Release are ref-counted.
		--initCount;
		if (initCount == 0)
		{
			//FCollada::Release();
		}
		break;

	case DLL_PROCESS_VERIFIER:
	default:
		break;
	}
	hInstance = _hInstance;
	return TRUE;
}
#elif defined(__APPLE__) || defined(LINUX)
void __attribute((constructor)) DllEntry(void)
{
}

void __attribute((destructor)) DllTerminate(void)
{
}
#endif // WIN32

#include "FMath/FMColor.h"
#include "FMath/FMRandom.h"
#include "FUDebug.h"
#include "FULogFile.h"
#include "FUBoundingBox.h"
#include "FUBoundingSphere.h"
#include "FCDocument.h"
#include "FCDAnimationClipTools.h"
#include "FCDLibrary.h"
#include "FCDSceneNode.h"
#include "FCDSceneNodeTools.h"
#include "FCDSceneNodeIterator.h"
#include "FCDParticleModifier.h"

// Trick the linker so that it adds the functionalities of the classes that are not used internally.
FCOLLADA_EXPORT void TrickLinker()
{
	// FMColor
	FMColor* color = NULL;
	float* f = NULL;
	color->ToFloats(f, 4);

	// FULogFile
	FULogFile* logFile = NULL;
	logFile->WriteLine("Test");

	// FUBoundingBox and FUBoundingSphere
	FUBoundingBox bb;
	FUBoundingSphere ss;
	if (!bb.Overlaps(ss))
	{
		// FUDebug
		DEBUG_OUT("Tricking Linker...");
	}

	// FCDAnimationClipTools
	FUObjectRef<FCDocument> d = FCollada::NewTopDocument();
	FCDAnimationClipTools::ResetAnimationClipTimes(d, 0.0f);

	// FCDSceneNodeTools
	FCDSceneNode* n = d->GetVisualSceneLibrary()->AddEntity();
	FCDSceneNodeIterator it(n);
	it.GetNode();
	FCDSceneNodeTools::GenerateSampledAnimation(n);
	FMMatrix44List mat44s = FCDSceneNodeTools::GetSampledAnimationMatrices();
	FCDSceneNodeTools::ClearSampledAnimation();

	//FCDParticleModifier* x = FCDEmitterParticle::CreateInstance(FCDParticleModifier::VELOCITY_ALIGN, *d);

	extern void TrickLinkerFCDLibrary();
	TrickLinkerFCDLibrary();
	extern void TrickLinker3();
	TrickLinker3(); // FCDSceneNodeIterator.
	extern void TrickLinkerFUParameter();
	TrickLinkerFUParameter();
	extern void TrickLinkerEffectParameter();
	TrickLinkerEffectParameter();
}

#endif // FCOLLADA_DLL

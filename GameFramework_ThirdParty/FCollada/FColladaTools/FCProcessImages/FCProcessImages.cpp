/*
    Copyright (C) 2005-2007 Feeling Software Inc.
    MIT License: http://www.opensource.org/licenses/mit-license.php

    Portions of the code surrounded by PREMIUM conditional compilation
    blocks are copyright Feeling Software Inc., strictly confidential and released
    to Premium Support licensees only. Licensees are allowed to modify and extend
    the Premium code for their internal use. Redistributing the Premium source code
    or any of its derivative product (e.g. binary versions) is forbidden.
*/

/*
	FCProcessImages processes the image library of a COLLADA document,
	removing the re-used images as well as transform them into DDS with mipmaps
	and power of 2 width/height.
*/

#include "StdAfx.h"
#include "FCDocument.h"
#include "FCDAsset.h"
#include "FCDEffect.h"
#include "FCDEffectProfileFX.h"
#include "FCDEffectParameter.h"
#include "FCDEffectParameterSurface.h"
#include "FCDEffectStandard.h"
#include "FCDEffectTechnique.h"
#include "FCDEffectTools.h"
#include "FCDImage.h"
#include "FCDLibrary.h"
#include "FCDMaterial.h"
#include "FUFileManager.h"

//#include <IL/il.h>
//#include <IL/ilu.h>

typedef fm::map<fstring, FCDImage*> FilenameMap;
typedef fm::map<FCDImage*, FCDImage*> ReplacementMap;

void ReplaceDuplicateImages(FCDImageLibrary* library);
void ProcessImages(FCDImageLibrary* library);
void ProcessImage(FCDImage* image);

void ProcessImageLibrary(FCDImageLibrary* library)
{
	ReplaceDuplicateImages(library);
	ProcessImages(library);
}

int main(int argc, const char* argv[], char* envp[])
{
#ifdef WIN32
	_environ = envp;
#else //LINUX
	environ = envp;
#endif //WIN32 and LINUX
	if (argc != 3)
	{
		std::cout << "Expecting two arguments:" << std::endl;
		std::cout << "FCProcessImages.exe <input_filename> <output_filename>" <<std::endl;
		exit(-1);
	}

	fstring inputFilename = TO_FSTRING(argv[1]);
	fstring outputFilename = TO_FSTRING(argv[2]);

	// Create an empty COLLADA document and import the given file.
	FCollada::Initialize();
	FUErrorSimpleHandler errorHandler;
	FCDocument* document = FCollada::NewTopDocument();
	FCollada::LoadDocumentFromFile(document, inputFilename.c_str());
	std::cout << argv[1] << std::endl;
	if (errorHandler.IsSuccessful())
	{
		std::cout << "Import: Done." << std::endl;

		// Initialize DevIL.
		ilInit();
		iluInit();

		// Process the image library
		ProcessImageLibrary(document->GetImageLibrary());

		// Shutdown DevIL
		ilShutDown();

		// It is common practice for tools to add a new contributor to identify that they were run
		// on a COLLADA document.
		FCDAssetContributor* contributor = document->GetAsset()->AddContributor();
		const char* userName = getenv("USER");
		if (userName == NULL) userName = getenv("USERNAME");
		if (userName != NULL) contributor->SetAuthor(TO_FSTRING(userName));
		contributor->SetSourceData(inputFilename);
		char authoringTool[1024];
		snprintf(authoringTool, 1024, "FCProcessImages sample for FCollada v%d.%02d", FCOLLADA_VERSION >> 16, FCOLLADA_VERSION & 0xFFFF);
		authoringTool[1023] = 0;
		contributor->SetAuthoringTool(TO_FSTRING((const char*)authoringTool));

		// Write out the processed COLLADA document.
		FCollada::SaveDocument(document, outputFilename.c_str());
		if (errorHandler.IsSuccessful())
		{
			std::cout << "Export: Done." << std::endl;
		}
		else
		{
			std::cout << errorHandler.GetErrorString();
			std::cout << std::endl << std::endl;
		}
	}
	else
	{
		std::cout << errorHandler.GetErrorString();
		std::cout << std::endl << std::endl;
	}

	SAFE_DELETE(document);
	FCollada::Release();
	return 0;
}

void ReplaceDuplicateImages(FCDImageLibrary* library)
{
	FilenameMap filenameMap;
	ReplacementMap replacementMap;

	// First step: make two maps. One for the original files and
	// one for the repeated images that will be replaced.
	size_t originalImageCount = library->GetEntityCount();
	for (size_t i = 0; i < originalImageCount; ++i)
	{
		FCDImage* image = library->GetEntity(i);
		const fstring& filename = image->GetFilename();
		FilenameMap::iterator itF = filenameMap.find(filename);
		if (itF != filenameMap.end())
		{
			replacementMap.insert(image, (*itF).second);
		}
		else
		{
			filenameMap.insert(filename, image);
		}
	}

	FCDEffectParameterList surfaces;
	if (!replacementMap.empty())
	{
		// Second step: iterate through all the effects/materials to find all the surface/texture parameters
		FCDMaterialLibrary* materialLibrary = library->GetDocument()->GetMaterialLibrary();
		for (size_t i = 0; i < materialLibrary->GetEntityCount(); ++i)
		{
			FCDMaterial* material = materialLibrary->GetEntity(i);
			size_t count = material->GetEffectParameterCount();
			for (size_t p = 0; p < count; ++p)
			{
				FCDEffectParameter* param = material->GetEffectParameter(p);
				if (param->IsType(FCDEffectParameterSurface::GetClassType())) surfaces.push_back(param);
			}
		}

		FCDEffectLibrary* effectLibrary = library->GetDocument()->GetEffectLibrary();
		for (size_t j = 0; j < effectLibrary->GetEntityCount(); ++j)
		{
			FCDEffect* effect = effectLibrary->GetEntity(j);
			size_t count = effect->GetEffectParameterCount();
			for (size_t p = 0; p < count; ++p)
			{
				FCDEffectParameter* param = effect->GetEffectParameter(p);
				if (param->IsType(FCDEffectParameterSurface::GetClassType())) surfaces.push_back(param);
			}

			for (size_t k = 0; k < effect->GetProfileCount(); ++k)
			{
				FCDEffectProfile* profile = effect->GetProfile(k);
				size_t count = profile->GetEffectParameterCount();
				for (size_t p = 0; p < count; ++p)
				{
					FCDEffectParameter* param = profile->GetEffectParameter(p);
					if (param->IsType(FCDEffectParameterSurface::GetClassType())) surfaces.push_back(param);
				}

				if (profile->GetObjectType().Includes(FCDEffectProfileFX::GetClassType()))
				{
					FCDEffectProfileFX* fxEffect = (FCDEffectProfileFX*) profile;
					for (size_t l = 0; l < fxEffect->GetTechniqueCount(); ++l)
					{
						FCDEffectTechnique* technique = fxEffect->GetTechnique(l);
						size_t count = technique->GetEffectParameterCount();
						for (size_t p = 0; p < count; ++p)
						{
							FCDEffectParameter* param = technique->GetEffectParameter(p);
							if (param->IsType(FCDEffectParameterSurface::GetClassType())) surfaces.push_back(param);
						}
					}
				}
			}
		}

		// Third step: replace all the duplicate images within these surfaces/textures
		for (FCDEffectParameterList::iterator itP = surfaces.begin(); itP != surfaces.end(); ++itP)
		{
			FCDEffectParameterSurface* surface = (FCDEffectParameterSurface*) (*itP);
			size_t imageCount = surface->GetImageCount();
			for (size_t c = 0; c < imageCount; ++c)
			{
				FCDImage* itI = surface->GetImage(c);
				ReplacementMap::iterator itR = replacementMap.find(itI);
				if (itR != replacementMap.end())
				{
					surface->RemoveImage(itI);
					surface->AddImage(itR->second, c);
				}
			}
		}

		// Fourth step: delete all the replaced images.
		for (ReplacementMap::iterator itR = replacementMap.begin(); itR != replacementMap.end(); ++itR)
		{
			(*itR).first->Release();
		}
	}
}

void ProcessImages(FCDImageLibrary* library)
{
	for (size_t i = 0; i < library->GetEntityCount(); ++i)
	{
		FCDImage* image = library->GetEntity(i);
		ProcessImage(image);
	}
}

void ProcessImage(FCDImage* image)
{
	// Create an image container in DevIL.
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	// Read in the image file into DevIL.
    fstring filename = image->GetFilename();
	if (!ilLoadImage(filename.c_str()))
	{
		std::cout << "Unable to read image file: '" << TO_STRING(filename).c_str() << "'." << std::endl;
		ilDeleteImages(1, &imageId);
		return;
	}

	// Retrieve the width/height of the image file.
	uint32 width = ilGetInteger(IL_IMAGE_WIDTH);
	uint32 height = ilGetInteger(IL_IMAGE_HEIGHT);
	uint32 depth = ilGetInteger(IL_IMAGE_DEPTH);
	if (width == 0 || height == 0 || depth == 0)
	{
		std::cout << "Invalid image file: '" << TO_STRING(filename).c_str() << "'." << std::endl;
		ilDeleteImages(1, &imageId);
		return;
	}

	// Check if the dimensions are power of 2
	uint16 countW=0, msbW=0, countH=0, msbH=0, countD=0, msbD=0;
	uint32 mask = 0x00000001;
	for (uint16 i=0; i<32; i++)
	{
		if ((width&mask) == mask) { countW++; msbW = i; }
		if ((height&mask) == mask) { countH++; msbH = i; }
		if ((depth&mask) == mask) { countD++; msbD = i; }
		mask = mask << 1;
	}

	// Round to the closest power of 2
	if (countW > 1) 
	{
		mask = 1 << (msbW-1);
		if ((width&mask) == mask)
			width = mask<<2;
		else
			width = mask<<1;
	}
	if (countH > 1)
	{
		mask = 1 << (msbH-1);
		if ((height&mask) == mask)
			height = mask<<2;
		else
			height = mask<<1;
	}
	if (countD > 1)
	{
		mask = 1 << (msbH-1);
		if ((depth&mask) == mask)
			depth = mask<<2;
		else
			depth = mask<<1;
	}
	
	// Resize image
	if ((countW | countH | countD) > 1)
	{
		iluImageParameter(ILU_FILTER, ILU_BILINEAR);
		iluScale(width, height, depth);
	}

	// Generate the mipmaps
	iluBuildMipmaps();

	// Write out the image, as a DDS file
	fstring extension = FUFileManager::GetFileExtension(filename);
	if (!extension.empty())
	{
		filename = filename.substr(0, filename.size() - extension.size() - 1);
	}
	filename += FC(".dds");
	remove(TO_STRING(filename).c_str()); // DevIL is strange: it won't replace a file.
	if (!ilSaveImage(filename.c_str()))
	{
		std::cout << "Unable to write processed image file: '" << TO_STRING(filename).c_str() << "'." << std::endl;
	}
	image->SetFilename(filename);

	// Clear the memory used for the image in DevIL.
	ilDeleteImages(1, &imageId);
}

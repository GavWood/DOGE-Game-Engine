////////////////////////////////////////////////////////////////////////////////
// ExStandardShader.cpp

#include "StdAfx.h"
#include "BaArchive.h"
#include "ExMaterial.h"
#include "ExTextureList.h"
#include "ExScene.h"
#include "FCDAsset.h"
#include "FCDocument.h"
#include "FCDController.h"
#include "FCDEffect.h"
#include "FCDEffectStandard.h"
#include "FCDLibrary.h"
#include "FCDLight.h"
#include "FCDGeometry.h"
#include "FCDGeometryInstance.h"
#include "FCDGeometryMesh.h"
#include "FCDGeometryPolygons.h"
#include "FCDGeometryPolygonsInput.h"
#include "FCDGeometryPolygonsTools.h"
#include "FCDGeometrySource.h"
#include "FCDSceneNode.h"
#include "FCDImage.h"
#include "FCDExtra.h"
#include "FCDTransform.h"
#include "FCDMaterialInstance.h"
#include "FCDMaterial.h"
#include "FCDMorphController.h"
#include "FCDPhysicsMaterial.h"
#include "FCDPhysicsModelInstance.h"
#include "FCDPhysicsRigidBody.h"
#include "FCDPhysicsRigidBodyInstance.h"
#include "FCDPhysicsRigidBodyParameters.h"
#include "FCDPhysicsScene.h"
#include "FCDPhysicsShape.h"
#include "FCDSkinController.h"
#include "FCDTexture.h"
#include "ExStandardShader.h"
#include "UtFilename.h"
#include "FsFile.h"

////////////////////////////////////////////////////////////////////////////////
// ExStandardShader

ExStandardShader::ExStandardShader( BtChar* pPath,
								    ExTextureList* pTextureList,
									ExMaterial* pMaterial )
{
	m_pPath = pPath;
	m_pTextureList = pTextureList;
	m_pMaterial = pMaterial;
	m_isEnvironmentMapped = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// ExStandardShader

void ExStandardShader::Export( const FCDEffectProfile* pEffectProfile )
{
    BtChar FSString[32];
    BtChar FS = FsFile::GetFileSeparator();
    sprintf( FSString, "%c", FS );
    
	const FCDEffectStandard* pStandardProfile = (FCDEffectStandard*) pEffectProfile;

	BtFloat opacity = pStandardProfile->GetOpacity();
    (void)opacity;
    
	RsColour v4DiffuseColour( RsColour::WhiteColour() );
	MtVector4 v4Transparency( 0, 0, 0, 0 );

	ExTexture* pDiffuseTexture = BtNull;
	ExTexture* pSpecularTexture = BtNull;

	if( pStandardProfile )
	{
		BtMemory::Copy( &v4DiffuseColour, &pStandardProfile->GetDiffuseColor(), sizeof(MtVector4) );
		BtMemory::Copy( &v4Transparency, &pStandardProfile->GetTranslucencyColor(), sizeof(MtVector4) );

		BtU32 nTextureCount;

		if( v4DiffuseColour.Alpha() != 1.0f )
		{
			// Has transparency
			int a=0;
			a++;
		}
		else if( v4Transparency.x != 1.0f )
		{
			v4DiffuseColour.Alpha( 1.0f - v4Transparency.x );
		}

		nTextureCount = (BtU32) pStandardProfile->GetTextureCount(FUDaeTextureChannel::DIFFUSE);

		if( nTextureCount > 0 )
		{
			if( pStandardProfile->GetTextureCount(FUDaeTextureChannel::DIFFUSE)>0)
			{
				const FCDTexture* pTexture = pStandardProfile->GetTexture(FUDaeTextureChannel::DIFFUSE,0);

				if( pTexture )
				{
					const FCDImage *pImage = pTexture->GetImage();

					if(pImage == BtNull)
					{
						ErrorLog::Fatal_Printf("Did not find diffuse texture image pointer.\n");
					}

					UtFilename originalTextureFilename((BtChar*)&pImage->GetFilename()[0]);

					BtFloat repeatU = 1;
					BtFloat repeatV = 1;
					BtFloat offsetU = 0;
					BtFloat offsetV = 0;
					BtFloat mirrorU = 0;
					BtFloat mirrorV = 0;
					BtFloat wrapU   = 0;
					BtFloat wrapV   = 0;

					// Cache the extra information
					const FCDExtra *pExtra = pTexture->GetExtra();

					// Get the number of types
					BtU32 extraCount = pExtra->GetTypeCount();

					// Get the extra types
					for( BtU32 i=0; i<extraCount; i++ )
					{
						// Cache each type
						const FCDEType *pEType = pExtra->GetType(i);

						if( pEType )
						{
							BtU32 techniqueCount = pEType->GetTechniqueCount();
						
							for( BtU32 j=0; j<techniqueCount; j++ )
							{
								const FCDETechnique *pTechnique = pEType->GetTechnique(j);

								if( pTechnique )
								{
									BtU32 childrenCount = pTechnique->GetChildNodeCount();

									for( BtU32 k=0; k<childrenCount; k++ )
									{
										// Cache each extra node
										const FCDENode* eNode = pTechnique->GetChildNode(k);

										if( eNode )
										{
											if (IsEquivalent((eNode)->GetName(), "mirrorU"))
											{
												const fchar *c = eNode->GetContent();
												mirrorU = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "mirrorV"))
											{
												const fchar *c = eNode->GetContent();
												mirrorV = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "wrapU"))
											{
												const fchar *c = eNode->GetContent();
												wrapU = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "wrapV"))
											{
												const fchar *c = eNode->GetContent();
												wrapV = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "repeatU"))
											{
												const fchar *c = eNode->GetContent();
												repeatU = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "repeatV"))
											{
												const fchar *c = eNode->GetContent();
												repeatV = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "offsetU" ) )
											{
												const fchar *c = eNode->GetContent();
												offsetU = (BtFloat)atof(c);
											}
											if (IsEquivalent((eNode)->GetName(), "offsetV" ) )
											{
												const fchar *c = eNode->GetContent();
												offsetV = (BtFloat)atof(c);
											}
										}
									}
								}
							}
						}
					}

					BtChar newTextureFilename[MaxFilePath];
					BtStrCopy( newTextureFilename, MaxFilePath, m_pPath );
					BtStrCat( newTextureFilename, MaxFilePath, FSString );
					BtStrCat( newTextureFilename,  MaxFilePath, originalTextureFilename.GetTitle() );
					BtStrCat( newTextureFilename, MaxFilePath, "." );
					BtStrCat( newTextureFilename,  MaxFilePath, originalTextureFilename.GetExtension() );

					// Get the texture filename
					UtFilename textureFilename( newTextureFilename );

					// Find the diffuse texture
					pDiffuseTexture = m_pTextureList->pFind( textureFilename.GetFilename() );

					// Add the texture
					if( pDiffuseTexture == BtNull )
					{
						pDiffuseTexture = new ExTexture();

						// Set the new filename
						pDiffuseTexture->GetFilename( textureFilename.GetFilename() );
						pDiffuseTexture->GetTitle( textureFilename.GetTitle() );

						BtBool isInverted = ExScene::IsMetasequoia();
						pDiffuseTexture->SetInverted(!isInverted);								// Invert model texture
						pDiffuseTexture->SetFlipped( BtFalse );
						pDiffuseTexture->IsMipMapped( BtTrue );
					}

					pDiffuseTexture->SetWrapU( wrapU );
					pDiffuseTexture->SetWrapV( wrapV );

					// Set the addressing mode
					m_pMaterial->SetRepeatU( repeatU );
					m_pMaterial->SetRepeatV( repeatV );
					
					m_pMaterial->SetOffsetU( offsetU );
					m_pMaterial->SetOffsetV( offsetV );

					// Set the texture index
					m_pMaterial->SetTextureIndex( DiffuseTextureIndex, m_pTextureList->Add( pDiffuseTexture ) );
				}
			}
		}

		nTextureCount = (BtU32) pStandardProfile->GetTextureCount(FUDaeTextureChannel::SPECULAR);

		if( nTextureCount > 0 )
		{
			if( pStandardProfile->GetTextureCount(FUDaeTextureChannel::SPECULAR)>0)
			{
				const FCDTexture* pTexture = pStandardProfile->GetTexture(FUDaeTextureChannel::SPECULAR,0);

				if( pTexture )
				{
					const FCDImage *pImage = pTexture->GetImage();

					if( pImage == BtNull )
					{
						ErrorLog::Fatal_Printf( "Did not find specular texture image pointer.\n" );
					}

					BtChar TextureFilename[MaxFilePath];
					BtStrCopy( TextureFilename, MaxFilePath, (BtChar*) &pImage->GetFilename()[0] );

					// Get the texture filename
					const BtChar* pName = strrchr( TextureFilename, '\\' ) + 1;

					BtChar SpecularFilename[MaxFilePath];
					sprintf( SpecularFilename, "%s%c%s", m_pPath, FS, pName );

					pSpecularTexture = m_pTextureList->pFind( SpecularFilename );

					// Add the texture
					if( pSpecularTexture == BtNull )
					{
						pSpecularTexture = new ExTexture();

						// Set the new filename
						pSpecularTexture->GetFilename( SpecularFilename );
						pSpecularTexture->GetTitle( pName );
						pSpecularTexture->SetInverted( BtTrue );
					}

					// Add the texture
					m_pMaterial->SetTextureIndex( SpecularTextureIndex, m_pTextureList->Add( pSpecularTexture ) );

					m_isEnvironmentMapped = BtTrue;
				}
			}
		}

	}

	m_pMaterial->DiffuseColour( v4DiffuseColour );
	m_pMaterial->SetFlag( RsMaterial_ZTest | RsMaterial_Lit | RsMaterial_Mipmap );
}

////////////////////////////////////////////////////////////////////////////////
/// DyCollisionAnalytical.cpp

#include <new>
#include "ErrorLog.h"
#include "BaResource.h"
#include "DyCollisionAnalytical.h"
#include "SgCollision.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void DyCollisionAnalytical::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	BtU8 *pInstanceMemory = (BtU8*)this;

	// Set the file data
	m_pFileData = ( BaCollisionAnalytical*) pFileData;

	// Advance past the file data
	pFileData += sizeof(BaCollisionAnalytical);

	// Set the pointer to pointer to shapes
	m_pShapes = (SgCollisionShape**) ( pInstanceMemory + sizeof(DyCollisionAnalytical) );

	// Set the number of shapes
	m_numShapes = m_pFileData->m_numShapes;

	pInstanceMemory += sizeof(DyCollisionAnalytical);
	pInstanceMemory += sizeof(BtU64) * m_numShapes;

	// Create the shapes
	for( BtU32 iShape=0; iShape<m_numShapes; iShape++ )
	{
		// Get the shape type
		SgCollisionType* pCollisionType = (SgCollisionType*) pFileData;

		// Go to the next shape type
        BtU32 collisionTypeSize = sizeof(SgCollisionType);
        (void)collisionTypeSize;
        
		pFileData += sizeof(SgCollisionType);

		// Switch the type of shape
		switch( *pCollisionType )
		{
			case SgCT_Box :
			
				m_pShapes[iShape] = new (pInstanceMemory) SgCollisionBox;

				// Go to the next shape
				pFileData += sizeof(BaSgCollisionBox);
				pInstanceMemory += sizeof(SgCollisionBox);
				break;

			case SgCT_Sphere :

				m_pShapes[iShape] = new (pInstanceMemory) SgCollisionSphere;

				// Go to the next shape
				pFileData += sizeof(BaSgCollisionSphere);
				pInstanceMemory += sizeof(SgCollisionSphere);
				break;

			case SgCT_Mesh :
				{
					m_pShapes[iShape] = new (pInstanceMemory)SgCollisionMesh;

					SgCollisionMesh *pMesh = (SgCollisionMesh*)m_pShapes[iShape];
					pMesh->m_pFileData = (BaSgCollisionMesh*)pFileData;

					// Go to the next shape
					pFileData += sizeof(BaSgCollisionMesh);
					pInstanceMemory += sizeof(SgCollisionMesh);
				}
				break;

			case SgCT_Cylinder:

				m_pShapes[iShape] = new (pInstanceMemory) SgCollisionCylinder;

				// Go to the next shape
				pFileData += sizeof(BaSgCollisionCylinder);
				pInstanceMemory += sizeof(SgCollisionCylinder);
				break;

			case SgCT_ConvexHull:
				{
					m_pShapes[iShape] = new (pInstanceMemory) SgCollisionHull;

					SgCollisionHull *pHull = (SgCollisionHull*)m_pShapes[iShape];
					pHull->m_pFileData = (BaSgCollisionMesh*)pFileData;
						
					// Go to the next shape
					pFileData += sizeof(BaSgCollisionMesh);
					pInstanceMemory += sizeof(SgCollisionHull);
				}
				break;

			default:
				break;	
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
// pCollisionShape

const SgCollisionShape* DyCollisionAnalytical::pCollisionShape( BtU32 iShape ) const
{
#ifdef _DEBUG
	if( iShape >= m_numShapes )
	{
		ErrorLog::Fatal_Printf( "DyCollisionAnalytical::pCollisionShape Shape index exceeds bound.\r\n" );
	}
#endif // _DEBUG

	// Return the shape
	return m_pShapes[iShape];
}

////////////////////////////////////////////////////////////////////////////////
// NumShapes

//virtual
BtU32 DyCollisionAnalytical::NumShapes() const
{
	return m_numShapes;
}

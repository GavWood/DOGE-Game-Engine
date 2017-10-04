////////////////////////////////////////////////////////////////////////////////
// ShCamera

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtQueue.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"

class RsTexture;
class BaArchive;
class RsMaterial;

// Class definition
class ShCamera
{
public:

	// Access
    static void                                 Create( BaArchive *pArchive );
    static void                                 Update( BtU32 textureA, BtU32 textureB );
    static void                                 Destroy();
    static void                                 Render();
 
	// State functions
    
	// Public functions
private:

    static BaArchive                           *m_pArchive;
    static RsMaterial                          *m_pMaterial;
};

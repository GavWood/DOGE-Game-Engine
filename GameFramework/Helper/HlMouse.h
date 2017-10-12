////////////////////////////////////////////////////////////////////////////////
// HlMouse

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"

class RsSprite;

// Class definition
class HlMouse
{
	public:

		// Public functions
		static void							Setup( BaArchive *pArchive );
		static void							SetScalar( BtFloat s );
		static void							Render( MtVector2 v2Dimension );
		static void							Render();

		// Accessors

	private:

		// Private functions

		// Private members
		static RsSprite						*m_pSprite;
		static MtVector2					 m_v2Position[32];
		static BtBool						 m_isVisible[32];
		static BtFloat						 m_scalar;
};

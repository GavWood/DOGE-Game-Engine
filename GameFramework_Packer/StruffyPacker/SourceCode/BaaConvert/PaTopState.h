////////////////////////////////////////////////////////////////////////////////
// PaTopState

#ifndef __PaTopState_h__
#define __PaTopState_h__

#include "BtSingleton.h"
#include "MCCollada.h"
#include "MCVanilla.h"
#include "BCxyz.h"
#include "BCdxf.h"
#include "BCRaw.h"
#include "MCMetasequoia.h"
#include "UtFilename.h"

class FCDocument;

// Class definition
class PaTopState : public BtSingleton<PaTopState> 
{
public:

	PaTopState();

	void						Main( int argc, _TCHAR* argv[] );

private:

	void						ConvertMQOToDAE( UtFilename* pFilename );
	void						ConvertDXFToDAE( UtFilename* pFilename );
	void						ConvertXYZToDAE( UtFilename* pFilename );
	void						ConvertXYZToMQO( UtFilename* pFilename );
	void						ConvertRawToMQO( UtFilename* pFilename );

	MCMetasequoia				m_metasequoia;
	BCxyz						m_xyz;
	BCRaw						m_raw;
	MCCollada					m_collada;
	BCdxf						m_dxf;
};

#endif // __PaTopState_h__

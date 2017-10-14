////////////////////////////////////////////////////////////////////////////////
// ExStandardShader

#ifndef __ExStandardShader_h__
#define __ExStandardShader_h__

#include "BtTypes.h"

class ExMaterial;
class FCDEffectProfile;
struct ExTextureList;

class ExStandardShader
{
public:

	// Constructor
	ExStandardShader( BtChar* pPath, ExTextureList* pTextureList, ExMaterial* pMaterial );

	// Public functions
	void					Export( const FCDEffectProfile* pEffectProfile );

	BtBool					IsEnvironmentMapped() const;

private:

	ExMaterial*				m_pMaterial;
	ExTextureList*			m_pTextureList;
	BtChar*					m_pPath;
	BtBool					m_isEnvironmentMapped;
};

inline BtBool ExStandardShader::IsEnvironmentMapped() const
{
	return m_isEnvironmentMapped;
}

#endif // __ExStandardShader_h__

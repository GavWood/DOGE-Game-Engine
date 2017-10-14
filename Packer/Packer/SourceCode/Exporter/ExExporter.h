////////////////////////////////////////////////////////////////////////////////
// ExExporter

#ifndef __ExExporter_h__
#define __ExExporter_h__

#include <vector>
#include "BtTypes.h"
#include "BtSingleton.h"
#include "FsFindFile.h"
#include "PaPacker.h"

class ExExporter : public BtSingleton< ExExporter >
{
public:

	ExExporter();

	BtBool					SetResourceType( const PaFileDetails &fileInfo, BaResourceType& eResourceType );

	void					compileResource( PaAsset* pAsset );

	BtBool					saveTextures() { return m_isSaveTextures; }
	BtBool					packTextures() { return m_isPackTextures; }

	PaAsset*				pCurrentAsset();

private:

	void					ExportUserInterface( PaAsset* pAsset );

	BtBool					m_isPackTextures;
	BtBool					m_isSaveTextures;

	PaAsset*				m_pCurrentAsset;
};

////////////////////////////////////////////////////////////////////////////////
// pCurrentAsset

inline PaAsset* ExExporter::pCurrentAsset()
{
	return m_pCurrentAsset;
}

#endif // __ExExporter_h__

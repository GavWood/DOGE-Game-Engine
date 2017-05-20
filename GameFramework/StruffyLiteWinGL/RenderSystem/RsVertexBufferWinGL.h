////////////////////////////////////////////////////////////////////////////////
/// RsVertexBufferWinGL.h

#pragma once
#include "BaFileData.h"
#include "BaResource.h"
#include "BaArchive.h"
#include "BtTypes.h"
#include <GL/glew.h>

// Class Declaration
class RsVertexBufferWinGL : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_VertexBuffer );

	RsVertexBufferWinGL();

	void							FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	void							CreateOnDevice();
	void							Copy( BtU8* pData, BtU32 nVertices );
	void							SetStream();
	void							RemoveFromDevice();

private:

	friend class RsImplWinDX;

	GLuint							m_vertexBuffer;
	BaVertexBufferFileData*			m_pFileData;
	BtU8*							m_pData;
	GLuint							m_vertexArray;
};

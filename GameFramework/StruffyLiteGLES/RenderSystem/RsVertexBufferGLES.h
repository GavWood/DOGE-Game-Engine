////////////////////////////////////////////////////////////////////////////////
/// RsVertexBufferWinGL.h

#ifndef __RsVertexBufferWinGL_h__
#define __RsVertexBufferWinGL_h__

#include "BaFileData.h"
#include "BaResource.h"
#include "BaArchive.h"
#include "BtTypes.h"
#include "RsGLES.h"

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
    BtBool                          IsDynamic();
    BtU8                           *GetStream();
    BtU32                           GetNumVertex();
    BtU32                           GetStride();
    
private:

	friend class RsImplWinDX;

	GLuint							m_vertexBuffer;
	BaVertexBufferFileData*			m_pFileData;
	BtU8*							m_pData;
};

#endif // __RsVertexBufferWinGL_h__

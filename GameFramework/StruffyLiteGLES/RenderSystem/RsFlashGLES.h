////////////////////////////////////////////////////////////////////////////////
/// RsFlashWinGL.h

#pragma once

#include "RsFlash.h"

class BaArchive;
class RsFlash;

struct BaFlashData;
struct cJSON;

// Class Declaration
class RsFlashWinGL : public RsFlash
{
public:

	void				FixPointers( BtU8 *pFileData, BaArchive *pArchive );

	void				CreateOnDevice();
	void				RemoveFromDevice();

	void				Reset();
	void				Update( BtFloat tick );
	void				Render( const MtVector2& v2Position, BtU32 sortOrder );

	// Accessors
	BtU32				GetFrameNumber();
	void				SetLooping( BtBool isLooping );
	BtBool				GetLooping();
    BtBool              IsFinished();

private:

	friend class BaArchiveManager;
	friend class RcRenderPipeline;

	BaFlashData		   *m_pFileData;
	cJSON			   *m_pJSON;
	BtU32				m_numFrames;
	BtU32				m_currentFrame;
	BtFloat				m_currentFrameTime;
	BtBool				m_isLooping;
    BtBool              m_isFinshed;
};

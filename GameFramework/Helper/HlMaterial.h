////////////////////////////////////////////////////////////////////////////////
// HlMaterial.h

// Include guard
#pragma once

#include "BtTypes.h"
#include "MtVector2.h"
#include "RsCamera.h"

class RsMaterial;

class HlMaterial
{
public:

	static void RenderFullscreen( RsMaterial *pMaterial, BtU32 sortorder );
	static void RenderUpsideDown( RsMaterial *pMaterial, const MtVector2 &v2Position, const MtVector2 &v2Dimension, const MtVector2 &v2ScreenDimension, BtU32 sortOrder );
    static void RenderUpsideDownXFlip( RsMaterial *pMaterial, const MtVector2 &v2Position, const MtVector2 &v2Dimension, const MtVector2 &v2ScreenDimension, BtU32 sortOrder );
	static void RenderRightWayUp( RsMaterial *pMaterial, const MtVector2 &v2Position, const MtVector2 &v2Dimension, const MtVector2 &v2ScreenDimension );
    static void RenderSideWays( RsMaterial *pMaterial, const MtVector2 &v2Position, const MtVector2 &v2Dimension );
	static void RenderUpscaledWithBorders(RsMaterial *pMaterial, BtU32 sortorder);

	static void SetRenderTarget(RsMaterial *pMaterial);
	static void RenderBillboard(RsCamera camera, RsMaterial *pMaterial);
    static void RenderSidewaysUpscaledWithBorders(RsMaterial *pMaterial, BtU32 sortorder);

private:

};

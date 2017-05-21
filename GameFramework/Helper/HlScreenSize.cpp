////////////////////////////////////////////////////////////////////////////////
// HlScreenSize

// Includes
#include "HlScreenSize.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "ApConfig.h"

// statics

////////////////////////////////////////////////////////////////////////////////
// GetBorder

MtVector2 HlScreenSize::GetBorder()
{
	if( ApConfig::IsAppleTV() )
	{
		return MtVector2(90.0f, 60.0f);
	}
	return MtVector2(0, 0);
}

////////////////////////////////////////////////////////////////////////////////
// Refactor

MtVector2 HlScreenSize::Refactor( const MtVector2 &v2OriginalScreenSize,
                                  const MtVector2 &v2Coord )
{
	BtFloat x = (BtFloat)RsUtil::GetWidth()  / v2OriginalScreenSize.x;
	BtFloat y = (BtFloat)RsUtil::GetHeight() / v2OriginalScreenSize.y;

	MtVector2 v2NewDimension = MtVector2( MtFloor(v2Coord.x * x), MtFloor(v2Coord.y * y) );

	return v2NewDimension;
}

////////////////////////////////////////////////////////////////////////////////
// Refactor

MtVector2 HlScreenSize::Refactor( const MtVector2 &v2OriginalScreenSize,
                                  const MtVector2 &v2Coord,
                                  const MtVector2 &v2CurrentScreenDimension )
{
	BtFloat x = (BtFloat)v2CurrentScreenDimension.x / v2OriginalScreenSize.x;
	BtFloat y = (BtFloat)v2CurrentScreenDimension.y / v2OriginalScreenSize.y;

	MtVector2 v2NewDimension = MtVector2( MtFloor(v2Coord.x * x), MtFloor(v2Coord.y * y) );

	return v2NewDimension;
}

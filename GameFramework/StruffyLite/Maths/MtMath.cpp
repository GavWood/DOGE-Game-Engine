// Local includes
#include "BtTypes.h"
#include "MtMath.h"

////////////////////////////////////////////////////////////////////////////////
// MtProjectPoint

//static
MtVector3 MtProjectPointW( const MtVector4& v4Position,
						   const MtMatrix4& m4ViewProjection,
						   BtU32 viewportWidth,
						   BtU32 viewportHeight )
{
	// Multiply by the world to screen transform
	MtVector4 v4ScreenSpacePosition = v4Position * m4ViewProjection;

	// Make a 2d vector
	MtVector2 v2Position( v4ScreenSpacePosition.x, v4ScreenSpacePosition.y );

	BtFloat z = 1.0f / v4ScreenSpacePosition.w;

	// Maps -1.0f to 1.0f
	v2Position.x = v2Position.x * z;
	v2Position.y = v2Position.y * z;

	// Maps 0 to 2
	v2Position.x = v2Position.x + 1.0f;
	v2Position.y = v2Position.y + 1.0f;

	// Maps 0 to 1
	v2Position.x = v2Position.x * 0.5f;
	v2Position.y = v2Position.y * 0.5f;

	// Maps 1 to 0
	v2Position.y = 1.0f - v2Position.y;

	// Maps 0 to width, 0 to height
	MtVector3 v3Result;

	v3Result.x = v2Position.x * viewportWidth;
	v3Result.y = v2Position.y * viewportHeight;
	v3Result.z = v4ScreenSpacePosition.z;

	return v3Result;
}

////////////////////////////////////////////////////////////////////////////////
// MtProjectPointZ

//static
MtVector3 MtProjectPointZ( const MtVector4& v4Position,
						   const MtMatrix4& m4ViewProjection,
						   BtU32 viewportWidth,
						   BtU32 viewportHeight )
{
	// Multiply by the world to screen transform
	MtVector4 v4ScreenSpacePosition = v4Position * m4ViewProjection;

	// Make a 2d vector
	MtVector2 v2Position( v4ScreenSpacePosition.x, v4ScreenSpacePosition.y );

	BtFloat z = 1.0f / v4ScreenSpacePosition.z;

	// Maps -1.0f to 1.0f
	v2Position.x = v2Position.x * z;
	v2Position.y = v2Position.y * z;

	// Maps 0 to 2
	v2Position.x = v2Position.x + 1.0f;
	v2Position.y = v2Position.y + 1.0f;

	// Maps 0 to 1
	v2Position.x = v2Position.x * 0.5f;
	v2Position.y = v2Position.y * 0.5f;

	// Maps 1 to 0
	v2Position.y = 1.0f - v2Position.y;

	// Maps 0 to width, 0 to height
	MtVector3 v3Result;

	v3Result.x = v2Position.x * viewportWidth;
	v3Result.y = v2Position.y * viewportHeight;
	v3Result.z = v4ScreenSpacePosition.z;

	return v3Result;
}

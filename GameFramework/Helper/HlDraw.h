////////////////////////////////////////////////////////////////////////////////
// HlDraw.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "RsRenderTarget.h"
#include "MtAABB.h"
#include "MtMatrix4.h"

class RsMaterial;
class BaArchive;

// Class definition
class HlDraw
{
public:

	// Public functions
	static void							Setup( RsMaterial *pMaterial2,
											   RsMaterial *pMaterial3,
											   RsMaterial *pMaterial3NoZTest = BtNull );

	static void							SetZTest( BtBool isEnabled );
	static void							SetCrossColour( const RsColour &colour1, const RsColour &colour2, const RsColour &colour3 );

	static void							RenderLine( const MtVector2 &v2First, const MtVector2 &v2Second, const RsColour &colour, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderLine( const MtVector3 &v3Start, const MtVector3 &v3End, const RsColour &colour, BtU32 sortOrder = MaxSortOrders - 1);

	static void							RenderCross( const MtVector2 &v2Position, BtFloat width = 5.0f, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderCross( const MtMatrix4 &m4Transforn, BtFloat width = 0.5f, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderArrow( const MtVector2 &v2A, const MtVector2 &v2B, BtFloat width = 0.5f, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderArrow( const MtVector3 &v3A, const MtVector3 &v3B, RsColour colour, BtU32 sortOrder = MaxSortOrders - 1);
	static void							RenderTriangleStrip( const MtVector3 *pv3Position, BtU32 numVertex, const RsColour &colour, BtU32 sortOrder = MaxSortOrders - 1);
	static void							RenderCircle( const MtVector3 &v3Position, const RsColour &colour, BtFloat width = 0.5f, BtU32 sortOrder = MaxSortOrders - 1);
	static void							RenderCG( const MtVector3 &v3Position, BtFloat width = 0.5f, BtU32 sortOrder = MaxSortOrders - 1);
	static void							RenderQuad( const MtVector2 &v2Position, MtVector2 v2Dimension, RsColour colour, BtU32 sortOrder = MaxSortOrders - 1);
	static void							RenderRotatedQuad(const MtVector2 &v2Position, MtVector2 v2Dimension, BtFloat angle, RsColour colour, BtU32 sortOrder = MaxSortOrders - 1);
	static void							RenderAABB( const MtAABB &aabb, MtMatrix4 m4Transform, RsColour colour, BtU32 sortOrder = MaxSortOrders - 1);

	// Accessors

private:

	static void							RenderArrow( const MtVector3 &v3A, const MtVector3 &v3B, MtVector3 v3Axis, RsColour colour, BtU32 sortOrder);

	// Private functions

	// Private members
	static RsMaterial					*m_pMaterial2;
	static RsMaterial					*m_pMaterial3;
	static RsMaterial					*m_pMaterial3NoZTest;
	static BtBool						 m_ztest;
	static RsColour						 m_colour1;
	static RsColour						 m_colour2;
	static RsColour						 m_colour3;
	static BtFloat						 m_arrowThickness;
};

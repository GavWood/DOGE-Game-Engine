////////////////////////////////////////////////////////////////////////////////
// ExMatrix.h

#pragma once
#include "StdAfx.h"
#include "MtMatrix4.h"
#include "BtMemory.h"
#include "FUtils.h"
#include "FCollada.h"
#include "FMMatrix44.h"

class ExMatrix
{
public:

	static void ChangeCoordinateSystem( MtMatrix4& m4Transform );
	static MtMatrix4 Convert( const FMMatrix44& f4Matrix );
};

////////////////////////////////////////////////////////////////////////////////
// Convert

// static
inline MtMatrix4 ExMatrix::Convert( const FMMatrix44& f4Matrix )
{
	MtMatrix4 m4Transform;
	BtMemory::Copy( &m4Transform, &f4Matrix, sizeof(MtMatrix4) );
	return m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

// static
inline void ExMatrix::ChangeCoordinateSystem( MtMatrix4& m4Transform )
{
	m4Transform[0][2] = -m4Transform[0][2];
	m4Transform[1][2] = -m4Transform[1][2];
	m4Transform[2][0] = -m4Transform[2][0];
	m4Transform[2][1] = -m4Transform[2][1];
	m4Transform[3][2] = -m4Transform[3][2];
}

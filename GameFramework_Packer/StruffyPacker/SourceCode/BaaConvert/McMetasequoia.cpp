////////////////////////////////////////////////////////////////////////////////
// MCMetasequoia.cpp

// Includes
#include <stdio.h>
#include "BtString.h"
#include "BtTypes.h"
#include "ErrorLog.h"
#include "McMetasequoia.h"

////////////////////////////////////////////////////////////////////////////////
// GetToken

BtS32 MCMetasequoia::GetToken( const BtChar* tokenName, BtU32 iLine )
{
	// Find the vertex token
	for( BtS32 currentLine = iLine; currentLine<(BtS32)m_reader.GetNumLines(); ++currentLine )
	{
		const BtChar* isVertex = m_reader.GetToken( currentLine, 0 );

		if( BtStrCompare( isVertex, tokenName ) == BtTrue )
		{
			return currentLine;
			break;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// ReadMaterial

void MCMetasequoia::ReadMaterial( BtU32 iLine )
{
	BtU32 count;

	// Cache the number of materials
	BtU32 numMaterials = m_reader.GetTokenAsInteger( iLine, 1 );

	// Loop through the materials
	for( BtU32 iMaterial=0; iMaterial<numMaterials; iMaterial++ )
	{
		MCMaterial material;

		++iLine;

		// Copy the name
		BtStrCopy( material.m_name, 256, m_reader.GetTokenAsString( iLine, 0 ) );

		count = m_reader.GetBracktedExpression( iLine, "tex(" );

		if( count != -1 )
		{
			// Get the texture name
			const BtChar* pTextureName = m_reader.GetBracktedExpressionAsString( 0 );

			// Copy the texture name
			BtStrCopy( material.m_diffuseFilename, 256, pTextureName );
		}
		else
		{
			BtStrEmpty( material.m_diffuseFilename, 256 );
		}

		count = m_reader.GetBracktedExpression( iLine, "col(" );

		if( count != -1 )
		{
			BtFloat red    = m_reader.GetBracktedExpressionAsFloat( 0 );
			BtFloat green  = m_reader.GetBracktedExpressionAsFloat( 1 );
			BtFloat blue   = m_reader.GetBracktedExpressionAsFloat( 2 );
			BtFloat alpha  = m_reader.GetBracktedExpressionAsFloat( 3 );

			// Set the colour
			material.m_colour = RsColour( red, green, blue, alpha );
		}

		m_vanilla.AddMaterial( material );
	}
}

////////////////////////////////////////////////////////////////////////////////
// ReadObject

void MCMetasequoia::ReadObject( BtS32 iLine )
{
	BtS32 nextLine;
	MCObject object;

	// Cache each token
	BtStrCopy( object.m_name, 256, m_reader.GetTokenAsString( iLine, 1 ) );

	// Write to the error log
	ErrorLog::Printf( "Converting object %s\r\n", object.m_name );

	// Find the depth token
	nextLine = GetToken( "depth", iLine );

	if( nextLine == -1 )
	{
		object.m_depth = 0;
	}
	else
	{
		iLine = nextLine;

		// Get the depth
		object.m_depth = m_reader.GetTokenAsInteger( iLine, 1 );
	}

	// Find the vertex token
	nextLine = GetToken( "vertex", iLine );

	if( nextLine == -1 )
	{
		object.m_depth = 0;
	}
	else
	{
		iLine = nextLine;

		// Get the number of vertex
		BtU32 numVertex = m_reader.GetTokenAsInteger( iLine, 1 );

		// Increment the line number
		++iLine;

		for( BtU32 i=0; i<numVertex; i++ )
		{
			// Read in the coordinates
			BtFloat x = m_reader.GetTokenAsFloat( iLine, 0 );
			BtFloat y = m_reader.GetTokenAsFloat( iLine, 1 );
			BtFloat z = m_reader.GetTokenAsFloat( iLine, 2 );
/*
			x = x / 64.0f;
			y = y / 64.0f;
			z = z / 64.0f;

			BtFloat sx = x * sqrtf(1.0f - y * y * 0.5f - z * z * 0.5f + y * y * z * z / 3.0f);
			BtFloat sy = y * sqrtf(1.0f - z * z * 0.5f - x * x * 0.5f + z * z * x * x / 3.0f);
			BtFloat sz = z * sqrtf(1.0f - x * x * 0.5f - y * y * 0.5f + x * x * y * y / 3.0f);

			x = sx * 64.0f;
			y = sy * 64.0f;
			z = sz * 64.0f;
*/
			object.AddVertex( MtVector3( x, y, z ) );

			// Increment the line number
			++iLine;
		}
	}

	// Find the vertex token
	nextLine = GetToken( "face", iLine );

	if( nextLine == -1 )
	{
		object.m_depth = 0;
	}
	else
	{
		iLine = nextLine;

		// Get the number of faces
		BtU32 numFaces = m_reader.GetTokenAsInteger( iLine, 1 );

		// Increment the line number
		++iLine;

		for( BtU32 iFace=0; iFace<numFaces; iFace++ )
		{
			MCFace face;

			// Cache the number of index
			BtU32 numIndex = m_reader.GetTokenAsInteger( iLine, 0 );

			if( numIndex != 3 )
			{
				int a=0;
				a++;
			}

			BtS32 numValues = m_reader.GetBracktedExpression( iLine, "V(" );


			// Add each index
			for( BtU32 i=0; i<numIndex; i++ )
			{
				// Read in the index
				BtU32 index = (BtU32)m_reader.GetBracktedExpressionAsFloat( i );

				// Add the index
				face.m_oldIndex.push_back( index );
			}

			// Get the number of tokens belonging to the UV
			numValues = m_reader.GetBracktedExpression( iLine, "UV(" );

			if( numValues != -1 )
			{
				// Add each index
				for( BtS32 UVTokenIndex=0; UVTokenIndex<numValues; UVTokenIndex+=2 )
				{
					// Read in the index
					BtFloat u = (BtFloat)m_reader.GetBracktedExpressionAsFloat( UVTokenIndex + 0 );
					BtFloat v = (BtFloat)m_reader.GetBracktedExpressionAsFloat( UVTokenIndex + 1 );

					// Add the index
					MtVector2 v2UV( u, v );

					// Add the UV
					face.m_v2UV.push_back( v2UV );
				}
			}

			BtU32 index = 0;

			// Read the material
			numValues = m_reader.GetBracktedExpression( iLine, "M(" );

			if( numValues == -1 )
			{
				// If no material is specified, set material ID = 0
				index = 0;
			}
			else
			{
				// Read in the index
				index = (BtU32)m_reader.GetBracktedExpressionAsFloat( 0 );
			}

			// Set the index
			face.m_materialID = index;

			// Add the face
			object.AddFace( face );

			// Go to the next line
			++iLine;
		}
	}


	if( strcmp( object.m_name, "convex" ) == 0 )
	{
		int a=0;
		a++;
	}

/*
	BtChar dynamicsFilename[256];

	// Set the material filename
	sprintf_s( dynamicsFilename, 256, "%s\\%s.config", pPath(), pNode->pTitle() );

	UtTokeniser tokeniser;

	if( tokeniser.Read( dynamicsFilename ) == BtTrue )
	{
		ErrorLog::Printf( "Read dynamics config file for %s\r\n", dynamicsFilename );

		tokeniser.GetBool( "convexhull", isConvexHull );
		tokeniser.GetBool( "collisionMesh", isCollisionMesh );
		tokeniser.GetBool( "dynamicvb", m_isDynamicVB );
		isMass = tokeniser.GetFloat( "mass", mass );

		pNode->SetNodeType( ExSgType_RigidBody );
	}
*/
	// Calculate the face normals
	m_vanilla.CalculateFaceNormals( object );

	// Calculate the smoothed normals
	m_vanilla.CalculateSmoothedNormals( object );

	// Add the polygons
	m_vanilla.AddPolygons( object );

	// Add the object
	m_vanilla.AddObject( object );
}

////////////////////////////////////////////////////////////////////////////////
// ReadMetasequoiaFile

void MCMetasequoia::ReadMetasequoiaFile()
{
	m_vanilla.Empty();

	m_currentDepth = -1;

	// Cache the number of lines
	BtU32 numLines = m_reader.GetNumLines();

	for( BtU32 iLine=0; iLine<numLines; iLine++ )
	{
		// Cache each token
		const BtChar* pToken = m_reader.GetToken( iLine, 0 );

		// Read an object
		if( BtStrCompare( pToken, "Material" ) == BtTrue )
		{
			ReadMaterial( iLine );
		}

		// Read an object
		if( BtStrCompare( pToken, "Object" ) == BtTrue )
		{
			ReadObject( iLine );
		}
	}

	// Add a material if we have no materials
	if( m_vanilla.GetNumMaterials() == 0 )
	{
		MCMaterial material;
		material.m_colour = RsColour( 1, 1, 1 );
		BtStrEmpty( material.m_diffuseFilename, 256 );
		BtStrCopy( material.m_name, 256, "AutoGenerated", 256 );
		m_vanilla.AddMaterial( material );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Load

void MCMetasequoia::Load( const BtChar* metasequoiaFilename )
{
	// Read the Metasequoia document
	m_reader.Open( metasequoiaFilename );

	// Convert the file
	ReadMetasequoiaFile();
}

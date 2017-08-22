////////////////////////////////////////////////////////////////////////////////
// BCMetasequoiaExport.cpp

// Includes
#include <stdio.h>
#include "BtString.h"
#include "BtTypes.h"
#include "ErrorLog.h"
#include "BcMetasequoiaExport.h"

/*
Metasequoia Document
Format Text Ver 1.0

Scene {
	pos 0.0000 0.0000 1500.0000
		lookat 0.0000 0.0000 0.0000
		head -0.5236
		pich 0.5236
		ortho 0
		zoom2 5.0000
		amb 0.250 0.250 0.250
}
Object "obj1" {
	depth 0
		folding 0
		scale 1.000000 1.000000 1.000000
		rotation 0.000000 0.000000 0.000000
		translation 0.000000 0.000000 0.000000
		visible 15
		locking 0
		shading 1
		facet 59.5
		color 0.898 0.498 0.698
		color_type 0
		vertex 4 {
			-100.0000 0.0000 -100.0000
				100.0000 0.0000 -100.0000
				-100.0000 0.0000 100.0000
				100.0000 0.0000 100.0000
	}
	face 1 {
		4 V(0 1 3 2) UV(0.00000 0.00000 1.00000 0.00000 1.00000 1.00000 0.00000 1.00000)
	}
}
Eof
*/

////////////////////////////////////////////////////////////////////////////////
// Export

void BCMetasequoiaExport::Export( const MCVanilla* pVanilla, const BtChar* pFilename )
{
	FILE *f;

	f = fopen( pFilename, "w+" );

	if( f != BtNull )
	{
		// Write the header

		//Scene {
		//		pos 0.0000 0.0000 1500.0000
		//		lookat 0.0000 0.0000 0.0000
		//		head -0.5236
		//		pich 0.5236
		//		ortho 0
		//		zoom2 5.0000
		//		amb 0.250 0.250 0.250
		//}
		fprintf( f, "Metasequoia Document\n" );
		fprintf( f, "Format Text Ver 1.0\n\n" );
		fprintf( f, "Scene {\n" );
		fprintf( f, "\tpos 0.0000 0.0000 1500.0000\n" );
		fprintf( f, "\tlookat 0.0000 0.0000 0.0000\n" );
		fprintf( f, "\thead 0.0\n" );
		fprintf( f, "\tpich 0.0\n" );
		fprintf( f, "\tortho 0\n" );
		fprintf( f, "\tzoom2 3.1250\n" );
		fprintf( f, "\tamb 0 0 0\n" );
		fprintf( f, "}\n" );

		// Cache the number of objects
		BtU32 numObjects = pVanilla->GetNumObjects();

		// Loop through the objects
		for( BtU32 i=0; i<numObjects; i++ )
		{
			// Cache each object
			const MCObject& object = pVanilla->GetObject( i );

			//Object "obj1" {
			//	depth 0
			//		folding 0
			//		scale 1.000000 1.000000 1.000000
			//		rotation 0.000000 0.000000 0.000000
			//		translation 0.000000 0.000000 0.000000
			//		visible 15
			//		locking 0
			//		shading 1
			//		facet 59.5
			//		color 0.898 0.498 0.698
			//		color_type 0
			//		vertex 4 {
			//			-100.0000 0.0000 -100.0000
			//				100.0000 0.0000 -100.0000
			//				-100.0000 0.0000 100.0000
			//				100.0000 0.0000 100.0000
			//	}
			//	face 1 {
			//		4 V(0 1 3 2) UV(0.00000 0.00000 1.00000 0.00000 1.00000 1.00000 0.00000 1.00000)
			//	}
			//}

			// Write out the first object
			fprintf( f, "\nObject \"%s\" {\n", object.m_name );
			fprintf( f, "\tdepth %d\n", object.m_depth );
			fprintf( f, "\tfolding 0\n" );
			fprintf( f, "\tscale 1.000000 1.000000 1.000000\n" );
			fprintf( f, "\trotation 0.000000 0.000000 0.000000\n" );
			fprintf( f, "\ttranslation 0.000000 0.000000 0.000000\n" );
			fprintf( f, "\tvisible 15\n" );
			fprintf( f, "\tlocking 0\n" );
			fprintf( f, "\tshading 1\n" );
			fprintf( f, "\tfacet 59.5\n" );
			fprintf( f, "\tcolor 1 1 1\n" );
			fprintf( f, "\tcolor_type 0\n" );

			// Cache the number of vertex
			BtU32 numVertex = (BtU32)object.m_newVertex.size();

			fprintf( f, "\tvertex %d {\n", numVertex );

			for( BtU32 i=0; i<numVertex; i++ )
			{
				// Cache each vertex
				const MCVertex& vertex = object.m_newVertex[i];

				// Cache each position
				const MtVector3& v3Position = vertex.m_v3Position;

				// Write it out
				fprintf( f, "\t\t%.4f %.4f %.4f\n", v3Position.x, v3Position.y, v3Position.z );
			}
			fprintf( f, "\t}\n" );

			// Cache the number of faces
			BtU32 numFaces = (BtU32)object.m_faces.size();

			fprintf( f, "\tface %d {\n", numFaces );

			for( BtU32 iFace=0; iFace<numFaces; iFace++ )
			{
				// Cache each vertex
				const MCFace& face = object.m_faces[iFace];

				// Cache the number of index
				BtU32 numIndex = (BtU32)face.m_newIndex.size();

				// Write the index
				fprintf( f, "\t\t%d V(", numIndex );

				// Loop through the index
				for( BtU32 i=0; i<numIndex; i++ )
				{
					fprintf( f, "%d", face.m_newIndex[i] );

					if( i != ( numIndex- 1 ) )
					{
						fprintf( f, " " );
					}
				}

				// Close the index
				fprintf( f, ")\n" );
			}

			// Close the faces
			fprintf( f, "\t}\n" );

			// Close object
			fprintf( f, "}\n" );
		}

		// Close the file
		fprintf( f, "Eof\n" );

		// Close the file
		fclose( f );
	}

	int a=0;
	a++;
}

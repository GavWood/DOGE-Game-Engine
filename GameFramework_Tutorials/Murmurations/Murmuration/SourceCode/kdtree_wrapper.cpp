// Uses code based on Jose Luis Blanco (joseluisblancoc@gmail.com)
// KD Tree work.

#include <nanoflann.hpp>
#include "MuMurmuration.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace nanoflann;

// This is an exampleof a custom data set class
template <typename T>
struct PointCloud
{
	struct Point
	{
		T  x,y,z;
	};

	std::vector<Point>  pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline T kdtree_distance(const T *p1, const size_t idx_p2,size_t /*size*/) const
	{
		const T d0=p1[0]-pts[idx_p2].x;
		const T d1=p1[1]-pts[idx_p2].y;
		const T d2=p1[2]-pts[idx_p2].z;
		return d0*d0+d1*d1+d2*d2;
	}

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, int dim) const
	{
		if (dim==0) return pts[idx].x;
		else if (dim==1) return pts[idx].y;
		else return pts[idx].z;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

template <typename T>
void generateRandomPointCloud(PointCloud<T> &point, const size_t N, const T max_range = 10)
{
	std::cout << "Generating "<< N << " point cloud...";
	point.pts.resize(N);
	for (size_t i=0;i<N;i++)
	{
		point.pts[i].x = max_range * (rand() % 1000) / T(1000);
		point.pts[i].y = max_range * (rand() % 1000) / T(1000);
		point.pts[i].z = max_range * (rand() % 1000) / T(1000);
	}

	std::cout << "done\n";
}

template <typename num_t> void kdtree_demo( SbStarling *pFlock, const size_t N)
{
	PointCloud<num_t> cloud;

	// Generate points: into "cloud" from array
	// Generate points:
	cloud.pts.resize(N);
	for (size_t i = 0; i < N; i++)
	{
		SbStarling &starling = pFlock[i];
		cloud.pts[i].x = starling.v3Pos.x;
		cloud.pts[i].y = starling.v3Pos.y;
		cloud.pts[i].z = starling.v3Pos.z;
	}

	// Construct a kd-tree index:
	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
		PointCloud<num_t>,
		3 /* dim */
		> my_kd_tree_t;

	my_kd_tree_t   index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );
	index.buildIndex();

	// Find the neighbors of the swallows

	for (BtU32 i = 0; i < N; i++)
	{
		SbStarling &starling = pFlock[i];
	
		num_t query_pt[3];
		query_pt[0] = starling.v3Pos.x;
		query_pt[1] = starling.v3Pos.y;
		query_pt[2] = starling.v3Pos.z;

		size_t num_results = 7;
		std::vector<size_t>   ret_index(num_results);
		std::vector<num_t> out_dist_sqr(num_results);
		num_results = index.knnSearch(&query_pt[0], num_results, &ret_index[0], &out_dist_sqr[0]);

		// Add the neighbours to our array
		starling.neighbours.Empty();

		if (num_results)
		{
			if (num_results > 7)
			{
				num_results = 7;
			}
			for (BtU32 j = 0; j < num_results; j++)
			{
				BtU32 index = ret_index[j];
				SbStarling *pStarling = &pFlock[index];

				if (pStarling != &starling)
				{
					starling.neighbours.Add(pStarling);
				}
			}
		}

		int a = 0;
		a++;
	}

}

void doKDTree( SbStarling *pFlock, int numBoids )
{
	// Randomize Seed
	kdtree_demo<float>( pFlock, numBoids);
}

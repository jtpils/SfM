#include <algorithm>

#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
using namespace cv;
using namespace xfeatures2d;

#include "Pipeline.hpp"
#include "util.hpp"

void Pipeline::extract_features(const Images& images,CamFrames& cam_Frames,DescriptorsVec& descriptors_vec)
{
	Logger _log("Step 1 (features)");

	// // create a sift detector
	// const int    feature_num      = 800;
	// const int    octavelayers_num = 3;
	// const double constrast_thresh = .04f;
	// const double edge_threshold   = 4.f;
	// const double sigma            = 1.3f;
	// Ptr<Feature2D> sift_detector = SIFT::create(feature_num,
	// 	octavelayers_num, constrast_thresh, edge_threshold, sigma);

	// create brist detector
	// parameters for brisk : int thresh = 30, int octaves = 3, float patternScale = 1.0f
	Ptr<Feature2D> brisk_detector = BRISK::create();

	// detect features in a loop
	for (int i = 0; i < images.size(); ++i)
	{
		Image image = images[i];

		KeyPoints key_points;
		Descriptors descriptors;
		// detects and computes descriptors

		//sift_detector->detectAndCompute(image.gray, noArray(), key_points, descriptors);
		brisk_detector->detectAndCompute(image.gray, noArray(), key_points, descriptors);

		// Remove 0-depth keypoints
		KeyPoints   keep_key_points;
		Descriptors keep_descriptors;
		for (size_t k = 0; k < key_points.size(); k++)
		{
			if (image.dep.at<float>(key_points[k].pt) != 0)
			{
				keep_key_points.push_back(key_points[k]);
				keep_descriptors.push_back(descriptors.row(k));
			}
		}

		// wrap keypoints to cam_Frame and add in to cam_Frames
		cam_Frames.push_back((CamFrame) {i, keep_key_points});
		descriptors_vec.push_back(keep_descriptors);

		//_log("Found %d key points in image %d.", key_points.size(), i);
	}

	_log.tok();
}

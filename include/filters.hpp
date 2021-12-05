#ifndef _SEQUOIA_FILTERS_HPP_
#define _SEQUOIA_FILTERS_HPP_

#include <opencv4/opencv2/opencv.hpp>

#define BLOB_MODE_MAXIMUM 1
#define BLOB_MODE_MINIMUM -1

/**
 * Removes elements of which the area is smaller than areaThreshold.
 * Performs the operation in-place.
 */
void cclSmallFilter(cv::Mat& input, const std::uint32_t areaThreshold);

/**
 * Finds the minimum or maximum blob in a cv::Mat, governed by
 * BLOB_MODE_{MAXIMUM,MINIMUM}
 */
cv::Mat blobFinder(cv::Mat input, const int type);

#endif  // _SEQUOIA_FILTERS_HPP_

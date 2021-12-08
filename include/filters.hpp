#ifndef _SEQUOIA_FILTERS_HPP_
#define _SEQUOIA_FILTERS_HPP_

#include <opencv4/opencv2/opencv.hpp>

#define BLOB_MODE_MAXIMUM 1
#define BLOB_MODE_MINIMUM -1

/**
 * Removes elements of which the area is smaller than areaThreshold.
 * Performs the operation in-place.
 * @TODO: Put in namespace
 */
void cclSmallFilter(cv::Mat& input, const std::uint32_t areaThreshold);

/**
 * Finds the minimum or maximum blob in a cv::Mat, governed by
 * BLOB_MODE_{MAXIMUM,MINIMUM}
 * @TODO: Put in namespace
 */
cv::Mat blobFinder(cv::Mat input, const int type);

namespace Sequoia::Image {
static inline const float HIST_H_RANGE[] = { 0, 180 };
static inline const float HIST_S_RANGE[] = { 0, 256 };
static inline const float HIST_V_RANGE[] = { 0, 256 };
static inline const int HIST_HS_CHANNELS[] = { 0, 1 };
static inline const float HIST_MAX_VAL = std::max({
    HIST_H_RANGE[1], HIST_S_RANGE[1], HIST_V_RANGE[1]
}) - 1;

/**
 * Channel selectors. Assumes HSV
 */
static inline const int HIST_SV_CHANNELS[] = { 1, 2 };

/**
 * Allowed ranges for the histogram. Assumes HSV.
 */
static inline const float* HIST_SV_RANGE[] = {
    Sequoia::Image::HIST_S_RANGE,
    Sequoia::Image::HIST_V_RANGE
};

/**
 * Histogram vector size. Assumes HSV.
 */
static inline const int HIST_SV_SIZE[] = {
    static_cast<int>(HIST_S_RANGE[1]),
    static_cast<int>(HIST_V_RANGE[1])
};

/**
 * Calculates the back projection given an input iamge and the histogram.
 * Assumes HSV.
 */
cv::Mat backProjectSV(cv::Mat input, cv::Mat model);

/**
 * Calculates a model based on the given inputs.
 * Assumes HSV.
 */
cv::Mat trainModelSV(cv::Mat* inputs, size_t inpCount);
}

#endif  // _SEQUOIA_FILTERS_HPP_

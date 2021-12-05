#include "filters.hpp"

#include <vector>
#include <boost/lexical_cast.hpp>

#define CV2DBG_WAIT_AFTER_IMSHOW
#include "cv2debugging.hpp"

#include "logging.hpp"
#include "context.hpp"

void cclSmallFilter(cv::Mat& input, const std::uint32_t areaThreshold) {
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int count = cv::connectedComponentsWithStats(
        input, labels, stats, centroids);

    imshowLabels(input, labels, count);

    cv::Mat noiseRemoved = cv::Mat::zeros(input.size(), input.depth());

    // TODO: Can be optimized by searching via labels, rather than exhaustive
    // search.
    for (int i = 0; i < noiseRemoved.rows; i++) {
        for (int j = 0; j < noiseRemoved.cols; j++) {
            const int label = labels.at<int>(i, j);
            const std::uint32_t area = stats.at<int>(
                    cv::Point(cv::CC_STAT_AREA, label));
            if (area > areaThreshold) {
                noiseRemoved.at<uchar>(i, j) = 255;
                continue;
            }

            noiseRemoved.at<uchar>(i, j) = 0;
        }
    }

    input = cv::Mat(noiseRemoved);
}

cv::Mat blobFinder(cv::Mat input, const int type = BLOB_MODE_MAXIMUM) {
    Context localContext;
    CTX_BLAME_ME(&localContext);

    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int count = cv::connectedComponentsWithStats(
        input, labels, stats, centroids);

    std::int64_t targetArea = 0;
    int targetLabel = 1;
    for (int i = 1; i < stats.rows; i++) {
        int areaT = stats.at<int>(cv::Point(cv::CC_STAT_AREA, i));
        // This is a nice cheat -- if you multiply by -1 the > operation becomes
        // a < operation, enabling us to handle two operations without two
        // functions.
        if (type * areaT > type * targetArea) {
            targetArea = areaT;
            targetLabel = i;
        }
    }

    return labels == targetLabel;
}

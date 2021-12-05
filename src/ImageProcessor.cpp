#include "ImageProcessor.hpp"
#include "config.hpp"
#include "trackhistogram.hpp"
#include "logging.hpp"
#include "filters.hpp"

#include <unistd.h>
#include <boost/lexical_cast.hpp>

ImageProcessor::ImageProcessor(Context* ctx, cv::Mat image) {
    this->context = ctx;
    this->image = image;
}

cv::Rect ImageProcessor::getBoundingBox(float threshold) {
    CTX_BLAME_ME(this->context);

    // Calculate the histogram backprojection.

    // Clean up noise with very small gaussian to not ruin the edge.
    cv::Mat blurred(this->image.size(), this->image.depth());
    cv::GaussianBlur(this->image, blurred,
            { DEF_BB_BLUR_SIZE, DEF_BB_BLUR_SIZE }, 2);

    cv::Mat hsvTarget(blurred.size(), blurred.depth());
    cv::cvtColor(blurred, hsvTarget, cv::COLOR_BGR2HSV);

    // Lumber Track Histogram
    cv::Mat refHistogram(hsvTarget.size(), hsvTarget.depth());
    cv::FileStorage histogramF(Configuration::getInstance().dataStore +
            "/trackHist.sequoia", cv::FileStorage::READ);
    histogramF["trackHist"] >> refHistogram;
    histogramF.release();

    cv::Mat backProj(hsvTarget.size(), hsvTarget.depth());

    TRACK_BACK_PROJECT(hsvTarget, refHistogram, backProj);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, { 5, 5 });

    cv::Mat output;
    cv::filter2D(backProj, output, -1, kernel);

    cv::Mat thresheld;
    const int maxVal = TRACK_HISTOGRAM_MAX_VAL;
    float relativeThreshold = threshold * static_cast<float>(maxVal);
    cv::threshold(output, thresheld, relativeThreshold, maxVal, 0);

    // Do an operation of closing
    cv::Mat closed = thresheld;
    cv::Mat closingKernel = cv::Mat::ones(DEF_BB_CLOSING_SIZE,
            DEF_BB_CLOSING_SIZE, 1);
    cv::erode(closed, closed, closingKernel);
    cv::dilate(thresheld, closed, closingKernel);

    // Do CCL and remove small objects.
    cv::Mat noiseRemoved = cv::Mat(closed);
    const int cclThreshold = static_cast<int>(
            (DEF_BB_AREA_THRESHOLD_PERCENT / 100.0) *
            closed.rows * closed.cols);
    cclSmallFilter(noiseRemoved, cclThreshold);

    // Find the biggest blob possible.
    cv::bitwise_not(noiseRemoved, noiseRemoved);
    cv::Mat boardBlob = blobFinder(noiseRemoved, BLOB_MODE_MAXIMUM);

    // Do another operation of closing
    cv::erode(boardBlob, boardBlob, closingKernel);
    cv::dilate(boardBlob, boardBlob, closingKernel);

    // Perform edge detection
    cv::Mat edges;
    cv::Canny(boardBlob, edges, 20, 250);

    cv::Mat result = edges;

    // Calculate the coordinates of the smallest inscribed rectangle.
    // This is the most pesimistic estimate.
    std::vector<cv::Point> coords;
    cv::findNonZero(result, coords);

    cv::Rect boundingRect = cv::minAreaRect(coords).boundingRect();
    return boundingRect;
}

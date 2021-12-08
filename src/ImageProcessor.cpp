#include "ImageProcessor.hpp"

#include <unistd.h>
#include <boost/lexical_cast.hpp>

#include "config.hpp"
#include "trackhistogram.hpp"
#include "logging.hpp"
#include "filters.hpp"
#include "cv2debugging.hpp"


ImageProcessor::ImageProcessor(Context* ctx, cv::Mat image) {
    this->context = ctx;
    this->image = image;
}

cv::Mat ImageProcessor::findDiscolorations() {
    CTX_BLAME_ME(this->context);

    // Clean up noise with very small gaussian to not ruin the edge.
    cv::Mat blurred(this->image.size(), this->image.depth());
    cv::GaussianBlur(this->image, blurred, DEF_DISCOLOR_BLUR_SIZE, 3);

    // Crop
    cv::Rect cropRect = cv::Rect(
        this->boardBox.x + DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.y + DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.width - 2 * DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.height - 2 * DEF_DISCOLOR_CROP_PADDING
    );
    cv::Mat cropped = blurred(cropRect);

    // Convert to HSV
    cv::Mat hsvTarget;
    cv::cvtColor(cropped, hsvTarget, cv::COLOR_BGR2HSV);

    // Known-Good Histogram
    cv::Mat refHistogram(hsvTarget.size(), hsvTarget.depth());
    cv::FileStorage histogramF(Configuration::getInstance().dataStore +
            "/boardHist.sequoia", cv::FileStorage::READ);
    histogramF["boardHist"] >> refHistogram;
    histogramF.release();

    cv::Mat backProj = Sequoia::Image::backProjectSV(hsvTarget, refHistogram);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, { 5, 5 });

    cv::Mat output;
    cv::filter2D(backProj, output, -1, kernel);

    cv::Mat thresheld;
    double maxVal, minVal;
    cv::minMaxLoc(output, &minVal, &maxVal);
    float relativeThreshold = 0.99 * static_cast<float>(maxVal);
    cv::threshold(output, thresheld, relativeThreshold, maxVal, 0);

    // Do an operation of closing
    cv::Mat closed = thresheld;
    cv::Mat closingKernel = cv::Mat::ones(DEF_BB_CLOSING_SIZE,
            DEF_BB_CLOSING_SIZE, 1);
    cv::erode(closed, closed, closingKernel);
    cv::dilate(closed, closed, closingKernel);

    cv::Mat result;
    cv::bitwise_not(closed, result);

    return result;
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

    // TODO: Implement this macro in ImageProcessor.hpp
    // @see ImageProcessor::backProjectSV
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
    this->boardBox = boundingRect;
    return boundingRect;
}

std::vector<cv::Vec3f> ImageProcessor::findKnots() {
    CTX_BLAME_ME(this->context);

    // Convert to Greyscale
    cv::Mat workFrame;
    cv::cvtColor(this->image, workFrame, cv::COLOR_BGR2GRAY);

    // TODO: Remove these magics
    cv::GaussianBlur(workFrame, workFrame, { 9, 9 }, 4);

    // Crop
    cv::Rect cropRect = cv::Rect(
        this->boardBox.x + DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.y + DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.width - 2 * DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.height - 2 * DEF_DISCOLOR_CROP_PADDING
    );
    workFrame = workFrame(cropRect);

    Sequoia::Dbg::saveImage(this->context, workFrame, "Preprocessed");

    std::vector<cv::Vec3f> circles;
    // TODO: Remove these magic numbers
    cv::HoughCircles(workFrame, circles, cv::HOUGH_GRADIENT_ALT, 1.5, 50, 300, 0.5, 2, 200);

    if (circles.size() == 0) {
        log_dbg(this->context, "Found no circles.");
    }

    return circles;
}

cv::Mat ImageProcessor::findEdges() {
    CTX_BLAME_ME(this->context);

    cv::Mat workFrame = cv::Mat(this->image);
    cv::GaussianBlur(workFrame, workFrame, { 9, 9 }, 4);

    // Crop
    cv::Rect cropRect = cv::Rect(
        this->boardBox.x + DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.y + DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.width - 2 * DEF_DISCOLOR_CROP_PADDING,
        this->boardBox.height - 2 * DEF_DISCOLOR_CROP_PADDING
    );
    workFrame = workFrame(cropRect);

    Sequoia::Dbg::saveImage(this->context, workFrame, "Preprocessed");

    cv::Mat out;
    cv::Canny(workFrame, out, 40, 50);

    return out;
}

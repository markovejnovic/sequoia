#ifndef _IMAGE_PROCESSOR_HPP_
#define _IMAGE_PROCESSOR_HPP_

#include <opencv4/opencv2/opencv.hpp>
#include "sequoiagfx/rectangle.hpp"
#include "context.hpp"

#define DEF_BB_THRESH_PERCENT 5
#define DEF_BB_BLUR_SIZE 7
#define DEF_BB_CLOSING_SIZE 5
#define DEF_BB_AREA_THRESHOLD_PERCENT 60.0
#define DEF_BB_AREA_TRACK_THRESHOLD_PERCENT 65.0

#define DEF_DISCOLOR_BLUR_SIZE  { 13, 13 }
#define DEF_DISCOLOR_CROP_PADDING 24

/**
 * Class handling image processing tasks. Uses OpenCV under-the-hood.
 */
class ImageProcessor {
 public:
    explicit ImageProcessor(Context* ctx, cv::Mat image);

    /**
     * Returns the Lumber bounding box.
     */
    cv::Rect getBoundingBox(float threshold = DEF_BB_THRESH_PERCENT / 100);

    /**
     * Returns a mask of all the detected discolorations in an image.
     */
    cv::Mat findDiscolorations();

 private:
    cv::Mat image;
    cv::Rect boardBox;
    Context* context;
};

#endif // _IMAGE_PROCESSOR_HPP_

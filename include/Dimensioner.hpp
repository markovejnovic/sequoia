#ifndef _SEQUOIA_DIMENSIONER_HPP_
#define _SEQUOIA_DIMENSIONER_HPP_

#include <opencv4/opencv2/opencv.hpp>

namespace Sequoia::Image {
class Dimensioner {
 public:
    explicit Dimensioner(int ppi) : ppi(static_cast<float>(ppi)) {}

    float pxToIn(int px);
    cv::Rect_<float> pxToIn(cv::Rect px);
    int inToPx(float in);
    cv::Rect inToPx(cv::Rect in);
 private:
    float ppi;
};
} // namespace Sequoia::Image

#endif

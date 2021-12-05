#ifndef _SEQUOIA_DRAWING_HPP_
#define _SEQUOIA_DRAWING_HPP_

#include <opencv4/opencv2/opencv.hpp>
#include "sequoiagfx/rectangle.hpp"
#include "sequoiagfx/color.hpp"

namespace Sequoia {
void drawRect(cv::Mat& target, Rectangle<int> rect, Color c);
}

#endif  // _SEQUOIA_DRAWING_HPP_

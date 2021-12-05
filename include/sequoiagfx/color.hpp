#ifndef _SEQUOIA_SEQUOIAGFX_COLOR_HPP_
#define _SEQUOIA_SEQUOIAGFX_COLOR_HPP_

#include <opencv4/opencv2/opencv.hpp>

#define SEQUOIA_BGR 0b001
#define SEQUOIA_RGB 0b100

namespace Sequoia {
struct Color { int r; int g; int b; };

cv::Scalar sequoia2cvColor(Color color, int mode);

inline static const Color LUMBER_BOUNDING_BOX_COLOR({ 255, 0, 0 });
}  // namespace Sequoia

#endif  // _SEQUOIA_SEQUOIAGFX_COLOR_HPP_ 

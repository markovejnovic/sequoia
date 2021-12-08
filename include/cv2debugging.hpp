#ifndef _SEQUOIA_CV2_DEBUGGING_HPP_
#define _SEQUOIA_CV2_DEBUGGING_HPP_

#include <opencv4/opencv2/opencv.hpp>
#include "context.hpp"

void imshowLabels(cv::Mat image, cv::Mat labels, int nLabels);

void imshow(cv::Mat image);

namespace Sequoia::Dbg {
void saveImage(Context* ctx, cv::Mat image, std::string name = "Unnamed");
}

#endif  // _SEQUOIA_CV2_DEBUGGING_HPP_

#include "sequoiagfx/image.hpp"

#include <vector>

cv::Mat Sequoia::Image::greyToChannel(cv::Mat input, std::uint8_t channel) {
    std::vector<cv::Mat> channels(3);
    for (size_t i = 0; i < 3; i++) {
        channels[i] = channel == i
            ? input
            : cv::Mat::zeros(input.size(), input.depth());
    }

    cv::Mat out;
    cv::merge(channels, out);
    return out;
}

#ifndef _SEQUOIA_IMAGE_HPP_
#define _SEQUOIA_IMAGE_HPP_

#include <opencv4/opencv2/opencv.hpp>

namespace Sequoia::Image {

    /**
     * Converts a greyscale image into a greyscale image in a specific channel.
     * @todo Implement better.
     */
    cv::Mat greyToChannel(cv::Mat input, std::uint8_t channel);
}

#endif  // _SEQUOIA_IMAGE_HPP_

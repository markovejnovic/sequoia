#include "sequoiagfx/color.hpp"

using Sequoia::Color;

cv::Scalar Sequoia::sequoia2cvColor(Color color, int mode) {
    switch (mode) {
        case SEQUOIA_RGB:
            return cv::Scalar(color.r, color.g, color.b);
    }
    return cv::Scalar(color.b, color.g, color.r);
}

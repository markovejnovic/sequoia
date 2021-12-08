#include "Dimensioner.hpp"

#include <cmath>

float Sequoia::Image::Dimensioner::pxToIn(int px) {
    return static_cast<float>(px) * (1.0 / this->ppi);
}

int Sequoia::Image::Dimensioner::inToPx(float in) {
    return static_cast<int>(std::round(ppi * in));
}

cv::Rect_<float> Sequoia::Image::Dimensioner::pxToIn(cv::Rect px) {
    float scale = (1.0 / this->ppi);
    return cv::Rect_<float> {
        scale * static_cast<float>(px.x),
        scale * static_cast<float>(px.y),
        scale * static_cast<float>(px.x + px.width),
        scale * static_cast<float>(px.y + px.height)
    };
}

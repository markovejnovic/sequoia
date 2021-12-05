#pragma once
#ifndef _SEQUOIA_RECTANGLE_HPP_
#define _SEQUOIA_RECTANGLE_HPP_

#include <opencv4/opencv2/opencv.hpp>

namespace Sequoia {
// Color-related operations.

template <class T> struct Rectangle {
    T x0;
    T y0;
    T x1;
    T y1;

    T width() { return x1 - x0; }
    T height() { return y1 - y1; }
};

template <class T> Rectangle<T> cv2SequoiaRect(cv::Rect rect) {
    return Rectangle<T> {
        rect.x,
        rect.y,
        rect.x + rect.width,
        rect.y + rect.height
    };
}

template <class T> Rectangle<T> cv2SequoiaRect(cv::Rect_<T> t) {
    return Rectangle<T> {
        t.x,
        t.y,
        t.x + t.width,
        t.y + t.height
    };
}

template <class T> cv::Rect sequoia2cvRect(Rectangle<T> rect) {
    return cv::Rect_<int>(
            cv::Point(rect.x0, rect.y0),
            cv::Point(rect.y0, rect.y1));
}

}  // namespace Sequoia

#endif  // _SEQUOIA_RECTANGLE_HPP_

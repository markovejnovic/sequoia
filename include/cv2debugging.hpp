#ifndef _SEQUOIA_CV2_DEBUGGING_HPP_
#define _SEQUOIA_CV2_DEBUGGING_HPP_

#ifdef SEQUOIA_STATIC_DEBUG

#include <vector>
#include <opencv4/opencv2/opencv.hpp>

void imshowLabels(cv::Mat image, cv::Mat labels, int nLabels) {
    std::vector<cv::Vec3b> colors(nLabels);
    colors[0] = cv::Vec3b(0, 0, 0);
    for (int label = 1; label < nLabels; label++) {
        colors[label] = cv::Vec3b(
            (rand() & 255), (rand() & 255), (rand() & 255));
    }

    cv::Mat showable(image.size(), CV_8UC3);
    for (int i = 0; i < showable.rows; i++) {
        for (int j = 0; j < showable.cols; j++) {
            int label = labels.at<int>(i, j);
            showable.at<cv::Vec3b>(i, j) = colors[label];
        }
    }

    imshow("imshowLabels", showable);
#ifdef CV2DBG_WAIT_AFTER_IMSHOW
    cv::waitKey();
#endif
}

void imshow(cv::Mat image) {
    imshow("imshow", image);
#ifdef CV2DBG_WAIT_AFTER_IMSHOW
    cv::waitKey();
#endif
}

#endif

#endif

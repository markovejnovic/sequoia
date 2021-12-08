#include "cv2debugging.hpp"

#include <vector>
#include <filesystem>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include "context.hpp"
#include "config.hpp"
#include "logging.hpp"

void imshowLabels(cv::Mat image, cv::Mat labels, int nLabels) {
#ifdef SEQUOIA_STATIC_DEBUG
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
#endif
}

void imshow(cv::Mat image) {
#ifdef SEQUOIA_STATIC_DEBUG
    imshow("imshow", image);
#ifdef CV2DBG_WAIT_AFTER_IMSHOW
    cv::waitKey();
#endif
#endif
}

void Sequoia::Dbg::saveImage(Context* ctx, cv::Mat image, std::string name) {
#if defined(SEQUOIA_STATIC_DEBUG) && defined(SAVE_IMAGES)
    if (Configuration::getInstance().isDebug) {
        std::string pathSt = Configuration::getInstance().dbgDir
            + "/" + ctx->blame + "_" + name + ".jpg";
        if (std::filesystem::exists(pathSt)) {
            log_warn(ctx, pathSt + " already exists. Overwriting.");
        } else {
            log_dbg(ctx, "Saving image to " + pathSt);
        }

        cv::Mat out = image;
        if (out.channels() == 1) {
            cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
        }

        cv::imwrite(pathSt, image);
    }
#endif
}


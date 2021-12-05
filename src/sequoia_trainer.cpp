#include <vector>

#include "config.hpp"
#include "context.hpp"
#include "logging.hpp"

#include "trackhistogram.hpp"
#include <opencv4/opencv2/opencv.hpp>

int main(int argc, char** argv) {
    Context *appCtx = new Context();
    Configuration& appCfg = loadConfig(appCtx);
    appCtx->isDebug = appCfg.isDebug;
    CTX_BLAME_ME(appCtx);

    std::string dataStore = appCfg.dataStore;

    if (argc <= 1) {
        log_err(appCtx, "Please provide the training image path.");
        return 1;
    }

    std::vector<cv::Mat> images;
    for (int i = 1; i < argc; i++) {
        cv::Mat image = cv::imread(argv[i]);

        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        images.push_back(hsv);
    }
    cv::Mat* cImages = &images[0];

    cv::Mat hist;
    cv::calcHist(images, { 0, 1 }, cv::noArray(), hist,
        { 180, 256 }, {0, 180, 0, 256});
    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
    TRACK_GEN_NORM_HISTOGRAM(cImages, argc - 1, hist);

    std::string path = dataStore + "/trackHist.sequoia";
    cv::FileStorage histogramF(path,
            cv::FileStorage::WRITE);

    if (!histogramF.isOpened()) {
        log_err(appCtx,
                "Could not open " + path + ". Do you have permissions?");
        return 1;
    }

    histogramF << "trackHist" << hist;

    histogramF.release();

    return 0;
}

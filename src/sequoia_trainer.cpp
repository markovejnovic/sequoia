#include <vector>

#include "config.hpp"
#include "context.hpp"
#include "logging.hpp"
#include "filters.hpp"

#include "trackhistogram.hpp"
#include <opencv4/opencv2/opencv.hpp>

#define ARG_NAME 1
#define ARG_MODE 2
#define ARG_IMG 3

int main(int argc, char** argv) {
    Context *appCtx = new Context();
    Configuration& appCfg = loadConfig(appCtx);
    appCtx->isDebug = appCfg.isDebug;
    CTX_BLAME_ME(appCtx);

    std::string dataStore = appCfg.dataStore;

    if (argc < 4) {
        log_err(appCtx, "Please provide the training image path and the "
                        "training name and the mode 'sv'/'hs'");
        return 1;
    }

    std::vector<cv::Mat> images;
    for (int i = ARG_IMG; i < argc; i++) {
        cv::Mat image = cv::imread(argv[i]);

        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        images.push_back(hsv);
    }
    cv::Mat* cImages = &images[0];

    std::string mode = std::string(argv[ARG_MODE]);
    cv::Mat hist;
    if (mode == "hs") {
        cv::calcHist(images, { 0, 1 }, cv::noArray(), hist,
            { 180, 256 }, {0, 180, 0, 256});
        cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
        TRACK_GEN_NORM_HISTOGRAM(cImages, images.size(), hist);
    } else if (mode == "sv") {
        hist = Sequoia::Image::trainModelSV(cImages, images.size());
    } else {
        log_err(appCtx, "I cannot identify mode: " + mode);
        return 1;
    }

    std::string path = dataStore + "/" + argv[ARG_NAME] + ".sequoia";
    cv::FileStorage histogramF(path,
            cv::FileStorage::WRITE);

    if (!histogramF.isOpened()) {
        log_err(appCtx,
                "Could not open " + path + ". Do you have permissions?");
        return 1;
    }

    histogramF << argv[ARG_NAME] << hist;

    histogramF.release();

    return 0;
}

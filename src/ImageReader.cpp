#include "ImageReader.hpp"

#include <chrono>
#include <string>

#include "AppThreadManager.hpp"
#include "logging.hpp"
#include "sequoiagfx/rectangle.hpp"
#include "sequoiagfx/drawing.hpp"

#include <boost/math/special_functions/round.hpp>

void ImageReader::bindProcessor(ImageProcessor *proc) {
    this->processor = proc;
}

void ImageReader::readProcessLoop(cv::Mat frame) {
    this->outputBuffer = new cv::Mat(frame);

    // This is where we actually process the image.
    cv::Mat workFrame;
    frame.copyTo(workFrame);

    ImageProcessor imageProcessor(this->ctx, workFrame);
    cv::Rect lumberBox = imageProcessor.getBoundingBox();

    cv::rectangle(*outputBuffer, lumberBox, cv::Scalar(0, 0, 255));
}

void ImageReader::main(WorkingContext* ctx) {
    this->ctx = ctx;
    this->capture = new cv::VideoCapture(ctx->videoPath);

    if (!this->capture->isOpened()) {
        log_err(ctx, "Could not open the camera.");
    }

    cv::Mat frame;
    auto prev = std::chrono::system_clock::now().time_since_epoch();
    while (this->capture->read(frame)) {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        auto time =
            std::chrono::duration_cast <std::chrono::milliseconds>(now - prev)
            / 1000.0;
        prev = now;
        if (time.count() != 0) {
            ctx->manager->updateFps(
                    round(1.0 / static_cast<float>(time.count())));
        }

        readProcessLoop(frame);

        ctx->manager->setOutputData(this->outputBuffer);
    }
}

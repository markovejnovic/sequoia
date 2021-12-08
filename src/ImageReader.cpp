#include "ImageReader.hpp"

#include <chrono>
#include <string>

#include "AppThreadManager.hpp"
#include "logging.hpp"
#include "sequoiagfx/rectangle.hpp"
#include "sequoiagfx/drawing.hpp"
#include "sequoiagfx/image.hpp"
#include "Dimensioner.hpp"
#include "config.hpp"
#include "units.hpp"

#include <boost/math/special_functions/round.hpp>

void ImageReader::bindProcessor(ImageProcessor *proc) {
    this->processor = proc;
}

void ImageReader::readProcessLoop(cv::Mat frame) {
    cv::Mat outBuf = cv::Mat(frame);

    // This is where we actually process the image.
    cv::Mat workFrame;
    frame.copyTo(workFrame);

    ImageProcessor imageProcessor(this->ctx, workFrame);

    // Let's find the bounding box of the image. The units of this are pixels,
    // so we need to convert to inches.
    cv::Rect lumberBox = imageProcessor.getBoundingBox();

    // TODO: PPI should be retrieved from context.
    cv::Rect_<float> inBox =
        Sequoia::Image::Dimensioner(Configuration::getInstance().camPpi)
        .pxToIn(lumberBox);
    int surfaceMeasure = Sequoia::Units::inin2sm(inBox.width, inBox.height);
    ctx->manager->updateMetrics(inBox.width, inBox.height, surfaceMeasure);

    // Draw the output
    cv::rectangle(outBuf, lumberBox, cv::Scalar(0, 0, 255));

    // Let us find discolorations in the image.
    cv::Mat discolorations = imageProcessor.findDiscolorations();

    // Draw the output
    cv::Mat cDisc = Sequoia::Image::greyToChannel(discolorations, 0);

    // TODO: Refactor the usage of DEF_DISCOLOR_CROP_PADDING away
    cv::copyMakeBorder(cDisc, cDisc,
        lumberBox.y + DEF_DISCOLOR_CROP_PADDING,
        frame.rows - (lumberBox.y + lumberBox.height) + DEF_DISCOLOR_CROP_PADDING,
        lumberBox.x + DEF_DISCOLOR_CROP_PADDING,
        frame.cols - (lumberBox.x + lumberBox.width) + DEF_DISCOLOR_CROP_PADDING,
        cv::BORDER_CONSTANT);
    Sequoia::Image::overlay({ outBuf, cDisc }, outBuf, 0.4);

    // Find knots
    std::vector<cv::Vec3f> knots = imageProcessor.findKnots();
    for (auto knot : knots) {
        cv::circle(outBuf, cv::Point {
                static_cast<int>(knot[0]),
                static_cast<int>(knot[1])
            }, knot[2], cv::Scalar(0, 255, 0));
    }

    // Find edges
    cv::Mat edges = imageProcessor.findEdges();
    cv::Mat cEdges = Sequoia::Image::greyToChannel(edges, 1);
    cv::copyMakeBorder(cEdges, cEdges,
        lumberBox.y + DEF_DISCOLOR_CROP_PADDING,
        frame.rows - (lumberBox.y + lumberBox.height) + DEF_DISCOLOR_CROP_PADDING,
        lumberBox.x + DEF_DISCOLOR_CROP_PADDING,
        frame.cols - (lumberBox.x + lumberBox.width) + DEF_DISCOLOR_CROP_PADDING,
        cv::BORDER_CONSTANT);
    Sequoia::Image::overlay({ outBuf, cEdges }, outBuf, 0.4);


    this->outputBuffer = new cv::Mat(outBuf);
    cv::waitKey();
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
            std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
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

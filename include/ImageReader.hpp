#ifndef _IMAGE_READER_HPP_
#define _IMAGE_READER_HPP_

#include <opencv4/opencv2/opencv.hpp>

#include "ImageProcessor.hpp"
#include "context.hpp"

class ImageReader {
 public:
    ImageReader() {}
    void bindProcessor(ImageProcessor* proc);
    void readProcessLoop(cv::Mat frame);
    void main(WorkingContext* ctx);

 private:
    ImageProcessor* processor;
    WorkingContext* ctx;
    cv::VideoCapture* capture;
    cv::Mat* outputBuffer;
};

#endif  // _IMAGE_READER_HPP_

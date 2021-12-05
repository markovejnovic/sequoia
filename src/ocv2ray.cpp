#include "ocv2ray.hpp"

PixelFormat ocvPixelFormatToRay(int depth, int channels) {
    switch (channels) {
        case 1:
            switch (depth) {
                case 1:
                    return PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
            }
        case 2:
            switch (depth) {
                case 1:
                    return PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA;
            }
        case 3:
            switch (depth) {
                case 3:
                    return PIXELFORMAT_UNCOMPRESSED_R8G8B8;
            }
    }

    return PIXELFORMAT_UNCOMPRESSED_R8G8B8;
}

Image ocvMatToRayImage(cv::Mat image) {
    cv::Mat rgbImage;
    cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);

    uchar* imgData = reinterpret_cast<uchar*>(
        malloc(rgbImage.total() * rgbImage.elemSize())
    );
    memcpy(imgData, rgbImage.data, rgbImage.total() * rgbImage.elemSize());

    return Image {
        imgData,
        rgbImage.cols,
        rgbImage.rows,
        1,
        PIXELFORMAT_UNCOMPRESSED_R8G8B8
    };
}

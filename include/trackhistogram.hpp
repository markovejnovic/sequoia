// > inb4 c++ developers start talking about inheritance and how this is
// > actually ugly
// > C's macros are the most beautiful creation man has invented.
// > im ready to die on that hill

#include <opencv4/opencv2/opencv.hpp>

#ifndef _SEQUOIA_TRACK_HISTOGRAM_HPP_
#define _SEQUOIA_TRACK_HISTOGRAM_HPP_

#ifndef TRACK_HISTOGRAM_VALUES
#define TRACK_HISTOGRAM_VALUES HS
#endif

#ifndef TRACK_HISTOGRAM_H_MAX
#define TRACK_HISTOGRAM_H_MAX 180
#endif

#ifndef TRACK_HISTOGRAM_S_MAX
#define TRACK_HISTOGRAM_S_MAX 256
#endif

#ifndef TRACK_HISTOGRAM_H_RANGES
#define TRACK_HISTOGRAM_H_RANGES { 0, TRACK_HISTOGRAM_H_MAX }
#endif

#ifndef TRACK_HISTOGRAM_S_RANGES
#define TRACK_HISTOGRAM_S_RANGES { 0, TRACK_HISTOGRAM_S_MAX }
#endif

#if TRACK_HISTOGRAM_VALUES == HS
#define TRACK_HISTOGRAM_CHANNELS { 0, 1 }
#else
#error "The only supported histogram values are H-S"
#endif

#ifndef TRACK_HISTOGRAM_MAX_VAL
#define TRACK_HISTOGRAM_MAX_VAL 255
#endif

#define TRACK_HISTOGRAM_DEFS \
    const float hranges[] = TRACK_HISTOGRAM_H_RANGES; \
    const float sranges[] = TRACK_HISTOGRAM_S_RANGES; \
    const float* ranges[] = { hranges, sranges }; \
    const int histogram_size[] = { \
        TRACK_HISTOGRAM_H_MAX, \
        TRACK_HISTOGRAM_S_MAX \
    }; \
    const int channels[] = TRACK_HISTOGRAM_CHANNELS;

#define TRACK_BACK_PROJECT(input, refHistogram, output) { \
    TRACK_HISTOGRAM_DEFS \
    cv::calcBackProject(&input, 1, channels, refHistogram, \
            output, ranges, 1); \
}

#define TRACK_GEN_NORM_HISTOGRAM(input, inpCount, output) { \
    TRACK_HISTOGRAM_DEFS \
    cv::calcHist(input, inpCount, channels, cv::Mat(), output, 2, \
            histogram_size, ranges); \
    cv::normalize(output, output, 0, TRACK_HISTOGRAM_MAX_VAL, \
            cv::NORM_MINMAX); \
}


#endif  // _SEQUOIA_TRACK_HISTOGRAM_HPP_

#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include "context.hpp"

void log_dbg(Context *context, std::string message);

void log_info(Context *context, std::string message);

void log_warn(Context *context, std::string message);

void log_err(Context *context, std::string message);

std::string mat2ShapeS(cv::Mat&);

#endif // _LOGGING_HPP_

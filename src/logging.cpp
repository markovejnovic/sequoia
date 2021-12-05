#include "logging.hpp"

#include <iostream>
#include <fstream>
#include "config.hpp"
#include <boost/lexical_cast.hpp>

#define LOG_TAG_ERR "[\033[31m"
#define LOG_TAG_DBG "["
#define LOG_TAG_INFO "["
#define LOG_TAG_END "\033[0m]: "

void _log_info(Context *ctx, std::string message) {
    std::string sink = Configuration::getInstance().logSink;

    if (sink == "stdout") {
        std::cout << message << std::endl;
        return;
    }

    std::ofstream logFile;
    logFile.open(sink);

    if (!logFile.is_open()) {
        log_err(ctx, "An error occurred opening the log file " + sink + ".");
    }

    logFile << message << std::endl;
    logFile.close();
}

void _log_err(Context *ctx, std::string message) {
    std::string sink = Configuration::getInstance().errSink;

    if (sink == "stderr") {
        std::cerr << message << std::endl;
        return;
    }

    std::ofstream logFile;
    logFile.open(sink);

    if (!logFile.is_open()) {
        log_err(ctx, "An error occurred opening the log file " + sink + ".");
    }

    logFile << message << std::endl;
    logFile.close();
}

void log_dbg(Context *context, std::string message) {
    if (!context->isDebug) {
        return;
    }

    _log_info(context, LOG_TAG_DBG + context->blame + LOG_TAG_END + message);
}

void log_info(Context *context, std::string message) {
    _log_info(context, LOG_TAG_INFO + context->blame + LOG_TAG_END + message);
}

void log_err(Context *context, std::string message) {
    _log_err(context, LOG_TAG_ERR + context->blame + LOG_TAG_END + message);
}

std::string mat2ShapeS(cv::Mat& mat) {
    auto shape = mat.size();
    return "(" +
           boost::lexical_cast<std::string>(shape.width) + "," +
           boost::lexical_cast<std::string>(shape.height) +
           ")";
}

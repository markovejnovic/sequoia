// Copyright 2021 Marko Vejnovic

#ifndef __linux__
#error "Platforms other than Linux are unsupported."
#endif

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "logging.hpp"
#include "config.hpp"
#include "errors.h"

/**
 * Converts a key-value pair into appropriate values and feeds it into config.
 * Both the key and value are undefined and can be considered garbled after this
 * function.
 */
void parse_config_kv(Configuration& config,
                     std::string key,
                     std::string value) {
    if (key == "source") {
        config.videoSource = value;
        return;
    }

    if (key == "debug") {
        boost::algorithm::to_lower(value);
        config.isDebug = value == "true";
        return;
    }

    if (key == "log_info") {
        config.logSink = value;
        return;
    }

    if (key == "log_err") {
        config.errSink = value;
        return;
    }

    if (key == "data_path") {
        config.dataStore = value;
        return;
    }
}

const std::string getConfigPath(Context* ctx) {
    // Linux Specific
    struct passwd *pw = getpwuid(getuid());
    std::string homedir = std::string(pw->pw_dir);

    std::filesystem::path userConf = std::filesystem::path(homedir) /
        std::filesystem::path(".conf") /
        std::filesystem::path("sequoia.conf");
    if (std::filesystem::exists(userConf)) {
        return userConf;
    }

    return std::filesystem::path("/") / 
        std::filesystem::path("etc") / 
        std::filesystem::path("sequoia.conf");
}

Configuration& loadConfig(Context* ctx) {
    CTX_BLAME_ME(ctx);

    Configuration& config = Configuration::getInstance();

    // Config file
    const std::string configPath = getConfigPath(ctx);

    std::ifstream confFile;
    confFile.open(configPath);

    if (!confFile.is_open()) {
        log_err(ctx, "Could not open configuration file " + configPath);
        exit(ERROR_CONF_FILE);
    }

    std::string confLine;
    while (getline(confFile, confLine)) {
        size_t split = confLine.find("=");
        std::string key = confLine.substr(0, split);
        std::string value = confLine.substr(split + 1, std::string::npos);
        parse_config_kv(config, key, value);
    }

    return config;
}

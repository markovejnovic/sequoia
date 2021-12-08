// Copyright 2021 Marko Vejnovic

#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>
#include "context.hpp"

/**
 * Singleton class representing the application configuration.
 */
class Configuration {
 public:
    static Configuration& getInstance() {
        static Configuration instance;
        return instance;
    }

    std::string videoSource;
    bool isDebug = false;
    std::string dataStore;
    std::string logSink = "stdout";
    std::string errSink = "stderr";
    int camPpi = 1;

    Configuration(Configuration const&) = delete;
    void operator=(Configuration const&) = delete;

 private:
    Configuration() {}
};

/**
 * Returns the configuration file path.
 */
const std::string getConfigPath(Context*);

/**
 * Loads the configuration and returns it as a Configuration. The user is
 * responsible to delete the object.
 */
Configuration& loadConfig(Context*);

#endif // _CONFIG_HPP_

#include "ConfigLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool ConfigLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                configData[key] = value;
                // print key and value
                std::cout << key;
                std::cout << value;
            }
        }
    }
    return true;
}

std::string ConfigLoader::getString(const std::string& key, const std::string& defaultVal) {
    if (configData.find(key) != configData.end()) return configData[key];
    return defaultVal;
}

double ConfigLoader::getDouble(const std::string& key, double defaultVal) {
    if (configData.find(key) != configData.end()) {
        try {
            return std::stod(configData[key]);
        } catch (...) {
            // If conversion fails (e.g. text instead of number), return default
            return defaultVal;
        }
    }
    return defaultVal;
}

int ConfigLoader::getInt(const std::string& key, int defaultVal) {
    if (configData.find(key) != configData.end()) {
        try {
            return std::stoi(configData[key]);
        } catch (...) {
            return defaultVal;
        }
    }
    return defaultVal;
}

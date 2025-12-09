#pragma once
#include <string>
#include <map>

class ConfigLoader {
public:
    bool load(const std::string& filename);
    std::string getString(const std::string& key, const std::string& defaultVal);
    double getDouble(const std::string& key, double defaultVal);
    int getInt(const std::string& key, int defaultVal);

private:
    std::map<std::string, std::string> configData;
};
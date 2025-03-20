#pragma once
#include <iostream>
#include <map>
#include "singleton.h"

class SectionInfo
{
private:
    std::map<std::string, std::string> data_;

public:
    SectionInfo() { data_.clear(); }
    ~SectionInfo() { data_.clear(); }
    std::string operator[](const std::string &key)
    {
        if (data_.find(key) == data_.end())
            return "";

        return data_[key];
    }
    void set_data(std::map<std::string, std::string> data);
};

class ConfigManager : public Singleton<ConfigManager>
{
    friend class Singleton<ConfigManager>;

private:
    std::map<std::string, SectionInfo> config_map_;

private:
    ConfigManager();

public:
    ~ConfigManager()
    {
        config_map_.clear();
    };

    SectionInfo operator[](const std::string &section_name)
    {
        if (config_map_.find(section_name) == config_map_.end())
            return SectionInfo();

        return config_map_[section_name];
    }
};

extern ConfigManager &app_config;
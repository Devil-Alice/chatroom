#pragma once
#include <iostream>
#include <map>
#include "singleton.h"

class SectionInfo
{
private:
    std::map<std::string, std::string> data_;
    std::string str_;

public:
    SectionInfo() { data_.clear(); }
    ~SectionInfo() { data_.clear(); }
    
    std::map<std::string, std::string> &get_data() ;
    std::string get_str();
    void set_str(std::string str) ;
    std::string operator[](const std::string &key);
};

class ConfigManager : public Singleton<ConfigManager>
{
    friend class Singleton<ConfigManager>;

private:
    std::map<std::string, SectionInfo> config_map_;

private:
    ConfigManager();

public:
    ~ConfigManager();

    SectionInfo operator[](const std::string &section_name);
};

extern ConfigManager &app_config;
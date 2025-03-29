#include "config_manager.h"
#include <fstream>

ConfigManager &app_config = ConfigManager::instance();

std::map<std::string, std::string> &SectionInfo::get_data()
{
    return data_;
}

std::string SectionInfo::get_str()
{
    return str_;
}

void SectionInfo::set_str(std::string str)
{
    str_ = str;
}

std::string SectionInfo::operator[](const std::string &key)
{
    if (data_.find(key) == data_.end())
        return "";

    return data_[key];
}

ConfigManager::ConfigManager()
{
    // 配置文件也可以采用ini格式，配合boost的filesystem解析

    // 解析当前目录下的application_config.json
    Json::Reader reader;
    Json::Value root;

    std::ifstream ifs("./app_config.json");
    bool success = reader.parse(ifs, root);
    if (!success || !root.isObject())
    {
        std::cerr << "Failed to parse JSON file!" << std::endl;
        return;
    }

    // 获取key的字符串
    for (Json::Value::iterator it_i = root.begin(); it_i != root.end(); it_i++)
    {
        SectionInfo info;
        std::map<std::string, std::string> &data = info.get_data();

        // std::cout << it_i.key().asString() << std::endl;
        // 如果这个section对象是object才解析
        if (it_i->isObject())
        {
            for (Json::Value::iterator it_j = it_i->begin(); it_j != it_i->end(); it_j++)
            {
                // std::cout << "\t" << it_j.key().asString() << std::endl;
                data[it_j.key().asString()] = it_j->asString();
            }
        }

        info.set_str(it_i->toStyledString());
        config_map_[it_i.key().asString()] = info;
    }
}

ConfigManager::~ConfigManager()
{
    config_map_.clear();
};

SectionInfo ConfigManager::operator[](const std::string &section_name)
{
    if (config_map_.find(section_name) == config_map_.end())
        return SectionInfo();

    return config_map_[section_name];
}
#include "config_manager.h"
#include <fstream>

ConfigManager &app_config = ConfigManager::instance();

void SectionInfo::set_data(std::map<std::string, std::string> data)
{
    this->data_ = data;
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
        std::map<std::string, std::string> data;

        // std::cout << it_i.key().asString() << std::endl;
        for (Json::Value::iterator it_j = it_i->begin(); it_j != it_i->end(); it_j++)
        {
            // std::cout << "\t" << it_j.key().asString() << std::endl;
            data[it_j.key().asString()] = it_j->asString();
        }

        SectionInfo info;
        info.set_data(data);
        config_map_[it_i.key().asString()] = info;
    }
}

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include "global.h"

class ConfigManager : public Singleton<ConfigManager>
{
    friend class Singleton<ConfigManager>;
public:
    class SectionInfo
    {
    private:
        std::map<QString, QString> data_;

    public:
        SectionInfo() { data_.clear(); }
        ~SectionInfo() { data_.clear(); }
        QString operator[](const QString &key);
        void set_data(std::map<QString, QString> data);
    };

private:
    std::map<QString, SectionInfo> config_map_;
    ConfigManager();
public:
    ~ConfigManager();
    SectionInfo operator[](QString config_name);
};

extern std::shared_ptr<ConfigManager> app_config;

#endif // CONFIG_MANAGER_H

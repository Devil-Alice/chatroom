#include "config_manager.h"

std::shared_ptr<ConfigManager> app_config = ConfigManager::instance();

QString ConfigManager::SectionInfo::operator[](const QString &key)
{
    if (data_.find(key) == data_.end())
        return "";

    return data_[key];
}

void ConfigManager::SectionInfo::set_data(std::map<QString, QString> data)
{
    this->data_ = data;
}

ConfigManager::ConfigManager()
{
    // 解析应用目录下的配置
    QFile file("./app_config.json");

    if (!file.open(QFile::ReadOnly))
    {
        throw std::runtime_error("配置文件打开失败");
        return;
    }

    QByteArray json_data = file.readAll();
    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(json_data, &json_error);
    if (json_error.error != QJsonParseError::NoError)
    {
        throw std::runtime_error("JSON 解析错误：" + json_error.errorString().toStdString());
        return;
    }

    if (!json_doc.isObject())
    {
        throw std::runtime_error("JSON 数据不是对象");
        return;
    }

    QJsonObject json_obj = json_doc.object();

    // 获取key的字符串
    for (QJsonObject::iterator it_i = json_obj.begin(); it_i != json_obj.end(); it_i++)
    {
        std::map<QString, QString> data;

        QJsonObject inner_obj = it_i.value().toObject();
        for (QJsonObject::iterator it_j = inner_obj.begin(); it_j != inner_obj.end(); it_j++)
        {
            data[it_j.key()] = it_j.value().toString();
        }

        SectionInfo info;
        info.set_data(data);
        config_map_[it_i.key()] = info;
    }

    qDebug() << "配置文件解析成功";
}

ConfigManager::~ConfigManager()
{
    config_map_.clear();
}

ConfigManager::SectionInfo ConfigManager::operator[](QString config_name)
{
    if (config_map_.find(config_name) == config_map_.end())
    {
        return SectionInfo();
    }

    return config_map_[config_name];
}

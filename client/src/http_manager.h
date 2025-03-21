#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H
#include "global.h"

class HttpManager : public QObject, public Singleton<HttpManager>, public std::enable_shared_from_this<HttpManager>
{
    Q_OBJECT; // 开启信号和槽的宏
    friend class Singleton<HttpManager>;

private:
    QNetworkAccessManager network_manager_;
    HttpManager();

public:
    ~HttpManager();
    void post_request(QUrl url, QJsonObject request_json_obj, MODULE module, REQUEST_ID request_id);
signals:
    void signal_request_finished(MY_STATUS_CODE code, MODULE module, REQUEST_ID request_id, QString response);
    void signal_resigter_request_finished(MY_STATUS_CODE code, REQUEST_ID request_id, QString response);
private slots:
    void slot_request_finished(MY_STATUS_CODE code, MODULE module, REQUEST_ID request_id, QString response);
};

#endif // HTTP_MANAGER_H

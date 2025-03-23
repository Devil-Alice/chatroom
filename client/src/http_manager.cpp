#include "http_manager.h"

HttpManager::HttpManager()
{
    connect(this, &HttpManager::signal_request_finished, &HttpManager::slot_request_finished);
}

HttpManager::~HttpManager()
{
}

void HttpManager::post_request(QUrl url, QJsonObject request_json_obj, MODULE module, REQUEST_ID request_id)
{
    QByteArray json_data = QJsonDocument(request_json_obj).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(json_data.length()));

    auto self = shared_from_this();
    QNetworkReply *reply(network_manager_.post(request, json_data));
    // 注册回调函数
    connect(reply, &QNetworkReply::finished, [self, reply, module, request_id]() -> bool
            {
        // 出现错误
        if (reply->error() != QNetworkReply::NoError)
        {
            emit self->signal_request_finished(NETWORK_FAILED, module, request_id, "");
            qDebug() << reply->errorString();
            return false;
        }

        // 没出现错误
        QString response = reply->readAll();
        // 发送信号
        emit self->signal_request_finished(SUCCESS, module, request_id, response);
        reply->deleteLater();
        return true; });
}

void HttpManager::slot_request_finished(MY_STATUS_CODE code, MODULE module, REQUEST_ID request_id, QString response)
{
    if (module == MODULE::REGISTER)
    {
        emit signal_resigter_request_finished(code, request_id, response);
        return;
    }

    if (module == MODULE::LOGIN)
    {
        emit signal_login_request_finished(code, request_id, response);
    }

    return;
}
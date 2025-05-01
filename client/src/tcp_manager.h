#ifndef TCP_MANAGER_H
#define TCP_MANAGER_H
#include "global.h"

class ConnectoinInfo
{
private:
public:
    QString uid;
    QString host;
    int port;
    QString token;
};

class TcpManager : public QObject, public Singleton<TcpManager>, public std::enable_shared_from_this<TcpManager>
{
    Q_OBJECT;
    friend class Singleton<TcpManager>;

private:
    QString host_;
    uint port_;
    QTcpSocket socket_;
    QByteArray buffer_;
    bool flag_head_complete_;
    quint16 request_id_;
    quint16 message_length_;
    quint16 head_length_;
    TcpManager();
    void connect_to_server(ConnectoinInfo info);
    void receive_message();
    void send_message(REQUEST_ID request_id, QString message);

signals:
    void signal_connection_status(bool success);
    void signal_send_message(REQUEST_ID request_id, QString message);
    void signal_message_received(REQUEST_ID request_id, QString message);
    void signal_chat_login_finished(QJsonObject json_data);
    void signal_search_content_finished(QJsonObject json_data);
    void signal_send_friend_apply_finished(QJsonObject json_data);
    void signal_query_friend_apply_finished(QJsonObject json_data);
    void signal_handle_friend_apply_finished(QJsonObject json_data);
    void signal_notify_receive_friend_apply(QJsonObject json_data);

public slots:
    void slot_connect_to_server(ConnectoinInfo info);
    void slot_send_message(REQUEST_ID request_id, QString message);
    void slot_message_received(REQUEST_ID request_id, QString message);

public:
    ~TcpManager();
};

#endif // TCP_MANAGER_H

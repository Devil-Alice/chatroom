#include "tcp_manager.h"

TcpManager::TcpManager()
{

    // 初始化成员变量
    host_ = "";
    port_ = 0;
    buffer_ = "";
    flag_head_complete_ = false;
    request_id_ = REQUEST_ID::UNKNOWN;
    message_length_ = 0;
    head_length_ = sizeof(request_id_) + sizeof(message_length_);

    connect(&socket_, &QTcpSocket::connected, this, [this]()
            { 
        qDebug() << "connected to the server successfully";
        emit signal_connection_status(true); });

    connect(&socket_, &QTcpSocket::disconnected, this, [this]()
            {
        qDebug() << "server disconnected "; 
        emit signal_connection_status(false); });

    connect(&socket_, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, [this](QAbstractSocket::SocketError socket_error)
            {
        
        switch (socket_error) {
            case QAbstractSocket::ConnectionRefusedError:
                qDebug() << "连接被拒绝 (Connection Refused)";
                break;
            case QAbstractSocket::RemoteHostClosedError:
                qDebug() << "远程主机关闭了连接 (Remote host closed the connection)";
                break;
            case QAbstractSocket::HostNotFoundError:
                qDebug() << "找不到主机 (Host not found)";
                break;
            case QAbstractSocket::SocketAccessError:
                qDebug() << "访问 socket 出错 (Socket access error)";
                break;
            case QAbstractSocket::SocketResourceError:
                qDebug() << "资源耗尽 (Socket resource error)";
                break;
            case QAbstractSocket::SocketTimeoutError:
                qDebug() << "连接超时 (Connection timeout)";
                break;
            case QAbstractSocket::OperationError:
                qDebug() << "操作错误 (Operation error)";
                break;
            case QAbstractSocket::SslHandshakeFailedError:
                qDebug() << "SSL 握手失败 (SSL handshake failed)";
                break;
            case QAbstractSocket::UnsupportedSocketOperationError:
                qDebug() << "不支持的 socket 操作 (Unsupported socket operation)";
                break;
            case QAbstractSocket::UnknownSocketError:
                qDebug() << "未知错误 (Unknown error)";
                break;
            default:
                qDebug() << "其他错误 (" << socket_error << ")";
                break;
            } });

    // 读取就绪时，读数据
    connect(&socket_, &QTcpSocket::readyRead, this, &TcpManager::receive_message);
    // 当发送信号来临时，发送数据
    connect(this, &TcpManager::signal_send_message, this, &TcpManager::slot_send_message);

    // 收到消息的处理事件
    connect(this, &TcpManager::signal_message_received, this, &TcpManager::slot_message_received);
}

void TcpManager::connect_to_server(ConnectoinInfo info)
{
    host_ = info.host;
    port_ = info.port;
    socket_.connectToHost(host_, port_);
    return;
}

void TcpManager::receive_message()
{

    buffer_.append(socket_.readAll());
    QDataStream stream(&buffer_, QIODevice::ReadOnly);

    // 接收头部信息
    if (!flag_head_complete_)
    {
        // 头部信息如果不够，则返回继续接收
        if (buffer_.length() < head_length_)
            return;

        stream >> request_id_ >> message_length_;
        buffer_.remove(0, head_length_);
        flag_head_complete_ = true;

    }

    // qDebug() << "current message length: " << buffer_.length() << endl;
    // 消息长度不够，继续接收
    if (buffer_.length() < message_length_)
        return;

    // 全部接收完毕
    QString message = buffer_.mid(0, message_length_);
    buffer_.remove(0, message_length_);
    // 重置状态接收头部
    flag_head_complete_ = false;

    
    qDebug() << "request id: " << request_id_ << endl
            << "message length: " << message_length_ << endl;
    qDebug() << "message: " << message << endl;
    emit signal_message_received((REQUEST_ID)request_id_, message);
    return;
}

void TcpManager::send_message(REQUEST_ID request_id, QString message)
{

    // 构造stream，方便写入数据
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    // 为流设置大端（网络字节序一般为大端）
    data_stream.setByteOrder(QDataStream::ByteOrder::BigEndian);

    quint16 req_id = (quint16)request_id;
    quint16 msg_len = (quint16)(message.toUtf8().length());
    // 先发送请求id（2字节）
    // 再发送message长度（2字节）
    // 最后发送message
    data_stream << req_id << msg_len;

    // 不用 << 写入data_stream的原因：<< 写入的数据默认会进行本机的默认编码处理，如果本机的编码不是utf8那么写入的数据可能会改变
    data.append(message.toUtf8());

    // 发送
    socket_.write(data);
}

void TcpManager::slot_connect_to_server(ConnectoinInfo info)
{
    connect_to_server(info);
}

void TcpManager::slot_send_message(REQUEST_ID request_id, QString message)
{
    send_message(request_id, message);
}

void TcpManager::slot_message_received(REQUEST_ID request_id, QString message)
{

    // 将message转换为json对象
    QJsonDocument json_doc = QJsonDocument::fromJson(message.toUtf8());
    if (json_doc.isNull() || !json_doc.isObject())
    {
        QMessageBox::information(nullptr, "info", "json解析错误");
        return;
    }

    QByteArray array = json_doc.toJson();

    if (request_id == REQUEST_ID::CHAT_LOGIN)
    {
        emit signal_chat_login_finished(json_doc.object());
    }
    else if (request_id == REQUEST_ID::SEARCH_CONTENT)
    {
        emit signal_search_content_finished(json_doc.object());
    }
    else if (request_id == REQUEST_ID::SNED_FRIEND_APPLY)
    {
        emit signal_send_friend_apply_finished(json_doc.object());
    }
    else if (request_id == REQUEST_ID::QUERY_FRIEND_APPLY)
    {
        emit signal_query_friend_apply_finished(json_doc.object());
    }
    else if (request_id == REQUEST_ID::HANDLE_FRIEND_APPLY)
    {
        emit signal_handle_friend_apply_finished(json_doc.object());
    }
    else if (request_id == REQUEST_ID::NOTIFY_SEND_FRIEND_APPLY)
    {
        emit signal_notify_receive_friend_apply(json_doc.object());
    }


    
}

TcpManager::~TcpManager()
{
}

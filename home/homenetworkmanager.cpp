#include "homenetworkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

HomeNetworkManager::HomeNetworkManager(QObject *parent)
    : QObject(parent)
    , mTcpSocket(new QTcpSocket(this))
    , mHostName("192.168.124.129")
    , mPort(8053) {
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &HomeNetworkManager::onReadyRead);
    connect(mTcpSocket, &QTcpSocket::connected, this, &HomeNetworkManager::onConnected);
    connect(mTcpSocket, &QAbstractSocket::errorOccurred, this, &HomeNetworkManager::onErrorOccurred);
}

void HomeNetworkManager::sendUserListRequest() {
    QJsonObject json;
    json["type"] = "user_list";
    QJsonDocument jsonDoc(json);
    // 将 QJsonDocument 转换为字节数组，并在其末尾添加分隔符 "\r\n\r\n"
    QByteArray data = jsonDoc.toJson() + "\r\n\r\n";
    sendRequest(data);
}

void HomeNetworkManager::sendUserInfoRequest(const QString& accountID) {
    QJsonObject json;
    json["type"] = "user_info";
    json["id"] = accountID;
    QJsonDocument jsonDoc(json);
    // 将 QJsonDocument 转换为字节数组，并在其末尾添加分隔符 "\r\n\r\n"
    QByteArray data = jsonDoc.toJson() + "\r\n\r\n";
    sendRequest(data);
}

void HomeNetworkManager::sendRequest(const QJsonObject &jsonData) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();
    }
    QJsonDocument doc(jsonData);
    mTcpSocket->write(doc.toJson());
}

void HomeNetworkManager::sendRequest(const QByteArray &data) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();
    }
    mTcpSocket->write(data);
}

void HomeNetworkManager::connectToServer() {
    mTcpSocket->connectToHost(mHostName, mPort);
}

void HomeNetworkManager::onReadyRead() {
    QByteArray data = mTcpSocket->readAll();

    // 静态缓冲区，用于存储接收到的数据
    static QByteArray buffer;

    // 将新接收到的数据追加到缓冲区
    buffer.append(data);

    // 处理缓冲区中的数据，直到找不到分隔符为止
    while (buffer.contains("\r\n\r\n")) {
        // 找到分隔符的位置
        int index = buffer.indexOf("\r\n\r\n");

        // 提取分隔符之前的部分作为一个完整的消息
        QByteArray message = buffer.left(index).trimmed();

        // 从缓冲区中移除已经处理的消息
        buffer.remove(0, index + 4);

        // 解析 JSON 消息
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        QJsonObject json = jsonDoc.object();

        if (json["type"] == "user_list") {
            QJsonArray jsonArray = json["list"].toArray();
            for (const QJsonValue& value : jsonArray) {
                QJsonObject jsonObject = value.toObject();
                QString id = jsonObject["id"].toString();
                QString nickname = jsonObject["nickname"].toString();
                emit userListItemResponse(id, nickname);
            }
        }
        else if (json["type"] == "user_info") {
            QString id = json["id"].toString();
            QString nickname = json["nickname"].toString();
            emit selfInfoResponse(id, nickname);
        }
    }
}

void HomeNetworkManager::onConnected() {
    qDebug() << "Connected to homeserver";
    qDebug() << "Server address:" << mTcpSocket->peerAddress().toString();
    qDebug() << "Server port:" << mTcpSocket->peerPort();
}

void HomeNetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error: Connection refused.";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: Host not found.";
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "Error: Socket operation timed out.";
        break;
    case QAbstractSocket::NetworkError:
        qDebug() << "Error: Network error.";
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug() << "Error: Unknown socket error.";
        break;
    default:
        qDebug() << "Error:" << mTcpSocket->errorString();
        break;
    }
}

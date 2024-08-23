#include "homenetworkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

HomeNetworkManager::HomeNetworkManager(QObject *parent)
    : QObject(parent)
    , mTcpSocket(new QTcpSocket(this))
    , mHostName("113.45.223.108")
    , mPort(8053) {
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &HomeNetworkManager::onReadyRead);
    connect(mTcpSocket, &QTcpSocket::connected, this, &HomeNetworkManager::onConnected);
    connect(mTcpSocket, &QAbstractSocket::errorOccurred, this, &HomeNetworkManager::onErrorOccurred);
}

void HomeNetworkManager::sendUserListRequest() {
    QJsonObject json;
    json["type"] = "user_list";
    sendRequest(json);
}

void HomeNetworkManager::sendUserInfoRequest(const QString& accountID) {
    QJsonObject json;
    json["type"] = "user_info";
    json["user_id"] = accountID;
    sendRequest(json);
}

void HomeNetworkManager::sendRequest(const QJsonObject& jsonData) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();
    }
    QJsonDocument doc(jsonData);
    mTcpSocket->write(doc.toJson());
}

void HomeNetworkManager::connectToServer() {
    mTcpSocket->connectToHost(mHostName, mPort);
}

void HomeNetworkManager::onReadyRead() {
    QByteArray data = mTcpSocket->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject json = jsonDoc.object();

    if (json["type"] == "user_list") {
        QJsonArray jsonArray = json["list"].toArray();
        for (const QJsonValue& value : jsonArray) {
            QJsonObject jsonObject = value.toObject();
            QString id = jsonObject["id"].toString();
            QString nickname = jsonObject["nickname"].toString();
            emit userListItemResponse(id, nickname);
            qDebug() << id << nickname;
        }
    }
    else if (json["type"] == "user_info") {
        qDebug() << json["id"] << json["nickname"];
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

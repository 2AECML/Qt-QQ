#include "authnetworkmanager.h"
#include <QByteArray>

AuthNetworkManager::AuthNetworkManager(QObject *parent) : QObject(parent), mTcpSocket(new QTcpSocket(this)){
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &AuthNetworkManager::onReadyRead);
    connect(mTcpSocket, &QTcpSocket::connected, this, &AuthNetworkManager::onConnected);
    connect(mTcpSocket, &QAbstractSocket::errorOccurred, this, &AuthNetworkManager::onErrorOccurred);
}

AuthNetworkManager::~AuthNetworkManager() {
    mTcpSocket->close();
}

void AuthNetworkManager::sendLoginRequest(const QString &account, const QString &password) {
    QJsonObject json;
    json["type"] = "login";
    json["account"] = account;
    json["password"] = password;
    sendRequest(json);
}

void AuthNetworkManager::sendRegisterRequest(const QString &nickname, const QString &password, const QString &phone, const QString &vcode) {
    QJsonObject json;
    json["type"] = "register";
    json["nickname"] = nickname;
    json["password"] = password;
    json["phone"] = phone;
    json["vcode"] = vcode;
    sendRequest(json);
}

bool AuthNetworkManager::isConnected() const {
    return mTcpSocket->state() == QAbstractSocket::ConnectedState;
}

void AuthNetworkManager::disconnect() {
    if (mTcpSocket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Disconnected from server";
        mTcpSocket->write("CLOSE");
        mTcpSocket->waitForBytesWritten();
        mTcpSocket->close();
    }
}

void AuthNetworkManager::onReadyRead() {
    QByteArray data = mTcpSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    if (json["type"] == "login") {
        emit loginResponse(json["success"].toBool(), json["message"].toString());
        // qDebug() << json["message"].toString();
    }
    else if (json["type"] == "register") {
        emit registerResponse(json["success"].toBool(), json["message"].toString());
        // qDebug() << json["message"].toString();
    }
}

void AuthNetworkManager::onConnected() {
    qDebug() << "Connected to server";
    qDebug() << "Server address:" << mTcpSocket->peerAddress().toString();
    qDebug() << "Server port:" << mTcpSocket->peerPort();
}

void AuthNetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
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

void AuthNetworkManager::sendRequest(const QJsonObject& jsonData) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();
    }
    QJsonDocument doc(jsonData);
    mTcpSocket->write(doc.toJson());
}

void AuthNetworkManager::connectToServer() {
    mTcpSocket->connectToHost("113.45.223.108", 8052);
}

#include "basicwebnetworkmanager.h"
#include <QJsonDocument>

BasicWebNetworkManager::BasicWebNetworkManager(QUrl url, QObject* parent)
    : QObject(parent)
    , mWebSocket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))
    , mUrl(url) {

    connect(mWebSocket, &QWebSocket::connected, this, &BasicWebNetworkManager::onConnected);
    connect(mWebSocket, &QWebSocket::disconnected, this, &BasicWebNetworkManager::onDisconnected);
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &BasicWebNetworkManager::onTextMessageReceived);
    connect(mWebSocket, &QWebSocket::binaryMessageReceived, this, &BasicWebNetworkManager::onBinaryMessageReceived);
}

BasicWebNetworkManager::~BasicWebNetworkManager() {
    disconnectFromServer();
}

bool BasicWebNetworkManager::isConnected() const {
    return mWebSocket->state() == QAbstractSocket::ConnectedState;
}

void BasicWebNetworkManager::disconnectFromServer() {
    if (mWebSocket->state() == QAbstractSocket::ConnectedState) {
        mWebSocket->close();
    }
    else {
        qDebug() << "Web Connection has already been closed";
    }
}

void BasicWebNetworkManager::connectToServer() {
    mWebSocket->open(mUrl);
}

void BasicWebNetworkManager::sendMessage(const QString& message) {
    if (mWebSocket->isValid()) {
        mWebSocket->sendTextMessage(message);
    } else {
        qWarning() << "Cannot send message, WebSocket is not connected.";
    }
}

void BasicWebNetworkManager::sendMessage(const QJsonObject& jsonObj) {
    if (mWebSocket->isValid()) {
        QJsonDocument doc(jsonObj);
        mWebSocket->sendBinaryMessage(doc.toJson());
    } else {
        qWarning() << "Cannot send message, WebSocket is not connected.";
    }
}

void BasicWebNetworkManager::sendMessage(const QByteArray& data) {
    if (mWebSocket->isValid()) {
        mWebSocket->sendBinaryMessage(data);
    } else {
        qWarning() << "Cannot send message, WebSocket is not connected.";
    }
}

void BasicWebNetworkManager::onConnected() {
    qDebug() << this->mWebSocket << "connected to server: " << mWebSocket->peerAddress().toString() << ":" << mWebSocket->peerPort();
}

void BasicWebNetworkManager::onDisconnected() {
    qDebug() << this->mWebSocket << "disconnected from server: " << mWebSocket->peerAddress().toString() << ":" << mWebSocket->peerPort();
}

void BasicWebNetworkManager::onTextMessageReceived(const QString& message) {
    qDebug() << this->mWebSocket << ": text message received: " << message;
}

void BasicWebNetworkManager::onBinaryMessageReceived(const QByteArray& data) {
    qDebug() << this->mWebSocket << ": binary data received: " << data;
}

void BasicWebNetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
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
        qDebug() << "Error:" << mWebSocket->errorString();
        break;
    }
}

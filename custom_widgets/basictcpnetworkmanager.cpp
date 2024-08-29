#include "basictcpnetworkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

BasicTcpNetworkManager::BasicTcpNetworkManager(const QString& hostName, const qint64 port, QObject* parent)
    : mTcpSocket(new QTcpSocket(this))
    , mHostName(hostName)
    , mPort(port) {

    connect(mTcpSocket, &QTcpSocket::connected, this, &BasicTcpNetworkManager::onConnected);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &BasicTcpNetworkManager::onDisconnected);
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &BasicTcpNetworkManager::onReadyRead);
    connect(mTcpSocket, &QAbstractSocket::errorOccurred, this, &BasicTcpNetworkManager::onErrorOccurred);
}

BasicTcpNetworkManager::~BasicTcpNetworkManager() {
    disconnectFromServer();
}

bool BasicTcpNetworkManager::isConnected() const {
    return mTcpSocket->state() == QAbstractSocket::ConnectedState;
}

void BasicTcpNetworkManager::disconnectFromServer() {
    if (mTcpSocket->state() == QAbstractSocket::ConnectedState) {
        mTcpSocket->write("CLOSE");
        mTcpSocket->waitForBytesWritten();
        mTcpSocket->close();
    }
    else {
        qDebug() << "Tcp Connection has already been closed";
    }
}

void BasicTcpNetworkManager::connectToServer() {
    mTcpSocket->connectToHost(mHostName, mPort);
}

void BasicTcpNetworkManager::sendRequest(const QJsonObject& jsonObj) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();
    }
    QJsonDocument doc(jsonObj);
    mTcpSocket->write(doc.toJson());
}

void BasicTcpNetworkManager::sendRequest(const QByteArray& data) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();
    }
    mTcpSocket->write(data);
}

QByteArray BasicTcpNetworkManager::appendDelimiter(const QJsonObject &jsonObj) {
    QJsonDocument jsonDoc(jsonObj);
    // 将 QJsonDocument 转换为字节数组，并在其末尾添加分隔符 "\r\n\r\n"
    QByteArray data = jsonDoc.toJson() + "\r\n\r\n";
    return data;
}

QList<QJsonObject> BasicTcpNetworkManager::extractDataToJsonList(const QByteArray& data) {
    // 储存Json的列表
    QList<QJsonObject> result;

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
        QJsonObject jsonObj = jsonDoc.object();

        result.append(jsonObj);
    }

    return result;
}

void BasicTcpNetworkManager::onReadyRead() {
    qDebug() << "Tcp: data received: " << mTcpSocket->readAll();
}

void BasicTcpNetworkManager::onConnected() {
    qDebug() << "Connected to server: " << mTcpSocket->peerAddress().toString() << ":" << mTcpSocket->peerPort();
}

void BasicTcpNetworkManager::onDisconnected() {
    qDebug() << "Disconnected from server: " << mTcpSocket->peerAddress().toString() << ":" << mTcpSocket->peerPort();
}

void BasicTcpNetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
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

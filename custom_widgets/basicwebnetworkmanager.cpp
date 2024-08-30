#include "basicwebnetworkmanager.h"
#include <QJsonDocument>

// 构造函数，初始化 WebSocket 网络管理器
BasicWebNetworkManager::BasicWebNetworkManager(QUrl url, QObject* parent)
    : QObject(parent)  // 调用基类构造函数
    , mWebSocket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))  // 创建一个新的 QWebSocket 对象
    , mUrl(url) {  // 服务器 URL

    // 连接 QWebSocket 的信号到槽函数
    connect(mWebSocket, &QWebSocket::connected, this, &BasicWebNetworkManager::onConnected);
    connect(mWebSocket, &QWebSocket::disconnected, this, &BasicWebNetworkManager::onDisconnected);
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &BasicWebNetworkManager::onTextMessageReceived);
    connect(mWebSocket, &QWebSocket::binaryMessageReceived, this, &BasicWebNetworkManager::onBinaryMessageReceived);
}

// 析构函数，断开连接
BasicWebNetworkManager::~BasicWebNetworkManager() {
    disconnectFromServer();  // 确保在销毁对象前断开连接
}

// 判断当前是否已连接到服务器
bool BasicWebNetworkManager::isConnected() const {
    return mWebSocket->state() == QAbstractSocket::ConnectedState;  // 检查 WebSocket 连接状态
}

// 断开与服务器的连接
void BasicWebNetworkManager::disconnectFromServer() {
    if (mWebSocket->state() == QAbstractSocket::ConnectedState) {
        mWebSocket->close();  // 关闭 WebSocket 连接
    }
    else {
        qDebug() << "Web Connection has already been closed";  // 如果已经断开，输出调试信息
    }
}

// 连接到服务器
void BasicWebNetworkManager::connectToServer() {
    mWebSocket->open(mUrl);  // 打开 WebSocket 连接到指定的 URL
}

// 发送文本消息
void BasicWebNetworkManager::sendMessage(const QString& message) {
    if (mWebSocket->isValid()) {
        mWebSocket->sendTextMessage(message);  // 发送文本消息
    } else {
        qWarning() << "Cannot send message, WebSocket is not connected.";  // 如果 WebSocket 无效，则输出警告信息
    }
}

// 发送 JSON 消息
void BasicWebNetworkManager::sendMessage(const QJsonObject& jsonObj) {
    if (mWebSocket->isValid()) {
        QJsonDocument doc(jsonObj);  // 将 JSON 对象转换为 QJsonDocument
        mWebSocket->sendBinaryMessage(doc.toJson());  // 发送 JSON 数据
    } else {
        qWarning() << "Cannot send message, WebSocket is not connected.";  // 如果 WebSocket 无效，则输出警告信息
    }
}

// 发送原始字节数据
void BasicWebNetworkManager::sendMessage(const QByteArray& data) {
    if (mWebSocket->isValid()) {
        mWebSocket->sendBinaryMessage(data);  // 发送原始字节数据
    } else {
        qWarning() << "Cannot send message, WebSocket is not connected.";  // 如果 WebSocket 无效，则输出警告信息
    }
}

// 处理连接成功事件
void BasicWebNetworkManager::onConnected() {
    qDebug() << this->mWebSocket << "connected to server: " << mWebSocket->peerAddress().toString() << ":" << mWebSocket->peerPort();  // 输出连接成功的信息
}

// 处理断开连接事件
void BasicWebNetworkManager::onDisconnected() {
    qDebug() << this->mWebSocket << "disconnected from server: " << mWebSocket->peerAddress().toString() << ":" << mWebSocket->peerPort();  // 输出断开连接的信息
}

// 处理接收到的文本消息
void BasicWebNetworkManager::onTextMessageReceived(const QString& message) {
    qDebug() << this->mWebSocket << ": text message received: " << message;  // 输出接收到的文本消息
}

// 处理接收到的二进制数据
void BasicWebNetworkManager::onBinaryMessageReceived(const QByteArray& data) {
    qDebug() << this->mWebSocket << ": binary data received: " << data;  // 输出接收到的二进制数据
}

// 处理网络错误事件
void BasicWebNetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error: Connection refused.";  // 连接被拒绝
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: Host not found.";  // 主机未找到
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "Error: Socket operation timed out.";  // 套接字操作超时
        break;
    case QAbstractSocket::NetworkError:
        qDebug() << "Error: Network error.";  // 网络错误
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug() << "Error: Unknown socket error.";  // 未知套接字错误
        break;
    default:
        qDebug() << "Error:" << mWebSocket->errorString();  // 其他错误
        break;
    }
}

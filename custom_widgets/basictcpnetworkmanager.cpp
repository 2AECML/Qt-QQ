#include "basictcpnetworkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// 构造函数，初始化网络管理器
BasicTcpNetworkManager::BasicTcpNetworkManager(const QString& hostName, const qint64 port, QObject* parent)
    : mTcpSocket(new QTcpSocket(this))  // 创建一个新的 QTcpSocket 对象
    , mHostName(hostName)  // 服务器主机名
    , mPort(port) {  // 服务器端口

    // 连接 QTcpSocket 的信号到槽函数
    connect(mTcpSocket, &QTcpSocket::connected, this, &BasicTcpNetworkManager::onConnected);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &BasicTcpNetworkManager::onDisconnected);
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &BasicTcpNetworkManager::onReadyRead);
    connect(mTcpSocket, &QAbstractSocket::errorOccurred, this, &BasicTcpNetworkManager::onErrorOccurred);
}

// 析构函数，断开连接
BasicTcpNetworkManager::~BasicTcpNetworkManager() {
    disconnectFromServer();  // 确保在销毁对象前断开连接
}

// 判断当前是否已连接到服务器
bool BasicTcpNetworkManager::isConnected() const {
    return mTcpSocket->state() == QAbstractSocket::ConnectedState;  // 检查 TCP 连接状态
}

// 断开与服务器的连接
void BasicTcpNetworkManager::disconnectFromServer() {
    if (mTcpSocket->state() == QAbstractSocket::ConnectedState) {
        mTcpSocket->write("CLOSE");  // 发送关闭连接的指令
        mTcpSocket->waitForBytesWritten();  // 等待指令发送完毕
        mTcpSocket->close();  // 关闭 TCP 连接
    }
    else {
        qDebug() << "Tcp Connection has already been closed";  // 如果已经断开，输出调试信息
    }
}

// 连接到服务器
void BasicTcpNetworkManager::connectToServer() {
    mTcpSocket->connectToHost(mHostName, mPort);  // 连接到指定的主机和端口
}

// 发送 JSON 请求
void BasicTcpNetworkManager::sendRequest(const QJsonObject& jsonObj) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();  // 如果未连接，则建立连接
    }
    QJsonDocument doc(jsonObj);  // 将 JSON 对象转换为 QJsonDocument
    mTcpSocket->write(doc.toJson());  // 发送 JSON 数据
}

// 发送原始字节数据
void BasicTcpNetworkManager::sendRequest(const QByteArray& data) {
    if (mTcpSocket->state() == QAbstractSocket::UnconnectedState) {
        connectToServer();  // 如果未连接，则建立连接
    }
    mTcpSocket->write(data);  // 发送原始字节数据
}

// 将 JSON 对象转换为字节数组并添加分隔符
QByteArray BasicTcpNetworkManager::appendDelimiter(const QJsonObject &jsonObj) {
    QJsonDocument jsonDoc(jsonObj);  // 将 JSON 对象转换为 QJsonDocument
    QByteArray data = jsonDoc.toJson() + "\r\n\r\n";  // 将 JSON 文档转换为字节数组，并添加分隔符
    return data;
}

// 从接收到的字节数据中提取 JSON 对象列表
QList<QJsonObject> BasicTcpNetworkManager::extractDataToJsonList(const QByteArray& data) {
    QList<QJsonObject> result;  // 储存 JSON 对象的列表

    static QByteArray buffer;  // 静态缓冲区，用于存储接收到的数据

    buffer.append(data);  // 将新接收到的数据追加到缓冲区

    while (buffer.contains("\r\n\r\n")) {
        int index = buffer.indexOf("\r\n\r\n");  // 查找分隔符的位置

        QByteArray message = buffer.left(index).trimmed();  // 提取分隔符之前的部分作为完整的消息

        buffer.remove(0, index + 4);  // 从缓冲区中移除已经处理的消息

        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);  // 解析 JSON 消息
        QJsonObject jsonObj = jsonDoc.object();  // 转换为 QJsonObject

        result.append(jsonObj);  // 将 JSON 对象添加到结果列表中
    }

    return result;
}

// 处理数据接收事件
void BasicTcpNetworkManager::onReadyRead() {
    qDebug() << "Tcp: data received: " << mTcpSocket->readAll();  // 读取并输出接收到的数据
}

// 处理连接成功事件
void BasicTcpNetworkManager::onConnected() {
    qDebug() << "Connected to server: " << mTcpSocket->peerAddress().toString() << ":" << mTcpSocket->peerPort();  // 输出连接成功的信息
}

// 处理断开连接事件
void BasicTcpNetworkManager::onDisconnected() {
    qDebug() << "Disconnected from server: " << mTcpSocket->peerAddress().toString() << ":" << mTcpSocket->peerPort();  // 输出断开连接的信息
}

// 处理网络错误事件
void BasicTcpNetworkManager::onErrorOccurred(QAbstractSocket::SocketError socketError) {
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
        qDebug() << "Error:" << mTcpSocket->errorString();  // 其他错误
        break;
    }
}

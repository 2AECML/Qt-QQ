#include "chatnetworkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QFile>
#include <QFileInfo>

// 构造函数，初始化网络连接和信号槽
ChatNetworkManager::ChatNetworkManager(const QString& selfID, QObject *parent)
    : mSelfID(selfID) // 保存用户ID
    , BasicWebNetworkManager(QUrl("ws://113.45.223.108:8080"), parent) { // 初始化基础网络管理器，连接到 WebSocket 服务器
    connectToServer(); // 连接到 WebSocket 服务器

    // 连接 WebSocket 的 connected 信号到发送用户ID的槽函数
    connect(mWebSocket, &QWebSocket::connected, this, &ChatNetworkManager::sendSelfID);
}

// 发送聊天消息
void ChatNetworkManager::sendChatMessage(const ChatMessage& chatMessage) {
    if (chatMessage.type == ChatMessage::Text) { // 如果消息类型是文本
        // 创建字体 JSON 对象
        QJsonObject fontObj;
        fontObj["family"] = chatMessage.font.family();
        fontObj["pointSize"] = chatMessage.font.pointSize();
        fontObj["weight"] = chatMessage.font.weight();
        fontObj["color"] = chatMessage.fontColor.name(QColor::HexRgb); // 使用 RGB 颜色

        // 创建消息 JSON 对象
        QJsonObject jsonObj;
        jsonObj["type"] = "text";
        jsonObj["senderId"] = chatMessage.senderID;
        jsonObj["receiverId"] = chatMessage.receiverID;
        jsonObj["time"] = chatMessage.timestamp.toString();
        jsonObj["font"] = fontObj;
        jsonObj["content"] = chatMessage.content;

        sendMessage(jsonObj); // 发送消息
    }
    else if (chatMessage.type == ChatMessage::Image || chatMessage.type == ChatMessage::File) { // 如果消息类型是图片或文件
        QFile file(chatMessage.content); // 打开文件

        if (!file.open(QIODevice::ReadOnly)) { // 如果文件无法打开
            qDebug() << "Failed to open file:" << chatMessage.content; // 输出调试信息
            qDebug() << "Error:" << file.errorString(); // 输出错误信息
        }

        qint64 fileSize = file.size(); // 获取文件大小
        qint64 offset = 0; // 文件偏移量
        int chunkNumber = 0; // 数据块编号
        int totalChunks = (fileSize + CHUNK_SIZE - 1) / CHUNK_SIZE; // 计算总数据块数

        // 读取文件的每一块
        while (offset < fileSize) {
            QByteArray chunk = file.read(CHUNK_SIZE); // 读取数据块
            QJsonObject metadata;

            // 创建文件元数据 JSON 对象
            metadata["fileName"] = QFileInfo(file.fileName()).fileName();
            metadata["fileSize"] = fileSize;
            metadata["chunkNumber"] = chunkNumber;
            metadata["totalChunks"] = totalChunks;

            QJsonObject jsonObj;
            jsonObj["type"] = (chatMessage.type == ChatMessage::Image) ? "image" : "file"; // 根据消息类型设置类型
            jsonObj["senderId"] = chatMessage.senderID;
            jsonObj["receiverId"] = chatMessage.receiverID;
            jsonObj["time"] = chatMessage.timestamp.toString();
            jsonObj["metadata"] = metadata;
            jsonObj["chunk"] = QString::fromUtf8(chunk.toBase64()); // Base64 编码数据块

            sendMessage(jsonObj); // 发送数据块

            offset += CHUNK_SIZE; // 更新偏移量
            chunkNumber++; // 更新数据块编号
        }
        file.close(); // 关闭文件
    }
}

// 发送用户ID到服务器
void ChatNetworkManager::sendSelfID() {
    if (mSelfID.isEmpty()) { // 如果用户ID为空
        return; // 不做任何操作
    }

    // 创建用户ID JSON 对象
    QJsonObject jsonObj;
    jsonObj["type"] = "clientId";
    jsonObj["value"] = mSelfID;

    sendMessage(jsonObj); // 发送用户ID
}

// 处理接收到的二进制消息
void ChatNetworkManager::onBinaryMessageReceived(const QByteArray &data) {
    ChatMessage chatMessage; // 创建聊天消息对象

    // 解析 JSON 消息
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj["type"] == "text") { // 如果消息类型是文本
        QString family = jsonObj["font"].toObject()["family"].toString();
        int pointSize = jsonObj["font"].toObject()["pointSize"].toInt();
        int weight = jsonObj["font"].toObject()["weight"].toInt();
        QColor fontColor = QColor(jsonObj["font"].toObject()["color"].toString());

        // 设置聊天消息属性
        chatMessage.type = ChatMessage::Text;
        chatMessage.senderID = jsonObj["senderId"].toString();
        chatMessage.receiverID = jsonObj["receiverId"].toString();
        chatMessage.timestamp = QDateTime::fromString(jsonObj["time"].toString());
        chatMessage.content = jsonObj["content"].toString();
        chatMessage.font = QFont(family, pointSize, weight);
        chatMessage.fontColor = fontColor;
    }
    else if (jsonObj["type"] == "image" || jsonObj["type"] == "file") { // 如果消息类型是图片或文件
        if (jsonObj["type"] == "image") {
            chatMessage.type = ChatMessage::Image;
        }
        else if (jsonObj["type"] == "file") {
            chatMessage.type = ChatMessage::File;
        }

        // 设置聊天消息属性
        chatMessage.senderID = jsonObj["senderId"].toString();
        chatMessage.receiverID = jsonObj["receiverId"].toString();
        chatMessage.timestamp = QDateTime::fromString(jsonObj["time"].toString());
        chatMessage.content = jsonObj["fileUrl"].toString();
        chatMessage.receivedFileName = jsonObj["fileName"].toString();

        qDebug() << "File name: " << chatMessage.receivedFileName; // 输出接收到的文件名
        qDebug() << "File url: " << chatMessage.content; // 输出接收到的文件 URL
    }
    else {
        return; // 如果消息类型不匹配，返回
    }

    emit messageReceived(chatMessage); // 发出消息接收到信号
}

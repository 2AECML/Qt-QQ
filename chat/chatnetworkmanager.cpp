#include "chatnetworkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QFile>
#include <QFileInfo>

ChatNetworkManager::ChatNetworkManager(const QString& selfID, QObject *parent)
    : mSelfID(selfID)
    , BasicWebNetworkManager(QUrl("ws://192.168.124.129:8080") , parent) {
    connectToServer();

    connect(mWebSocket, &QWebSocket::connected, this, &ChatNetworkManager::sendSelfID);
}

void ChatNetworkManager::sendChatMessage(const ChatMessage& chatMessage) {
    if (chatMessage.type == ChatMessage::Text) {
        QJsonObject fontObj;
        fontObj["family"] = chatMessage.font.family();
        fontObj["pointSize"] = chatMessage.font.pointSize();
        fontObj["weight"] = chatMessage.font.weight();
        fontObj["color"] = chatMessage.fontColor.name(QColor::QColor::HexRgb);

        QJsonObject jsonObj;
        jsonObj["type"] = "text";
        jsonObj["senderId"] = chatMessage.senderID;
        jsonObj["receiverId"] = chatMessage.receiverID;
        jsonObj["time"] = chatMessage.timestamp.toString();
        jsonObj["font"] = fontObj;
        jsonObj["content"] = chatMessage.content;

        sendMessage(jsonObj);
    }
    else if (chatMessage.type == ChatMessage::Image || chatMessage.type == ChatMessage::File){
        QFile file(chatMessage.content);

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file:" << chatMessage.content;
            qDebug() << "Error:" << file.errorString();
        }

        qint64 fileSize = file.size();
        qint64 offset = 0;
        int chunkNumber = 0;
        int totalChunks = (fileSize + CHUNK_SIZE - 1) / CHUNK_SIZE;

        while (offset < fileSize) {
            QByteArray chunk = file.read(CHUNK_SIZE);
            QJsonObject metadata;

            metadata["fileName"] = QFileInfo(file.fileName()).fileName();
            metadata["fileSize"] = fileSize;
            metadata["chunkNumber"] = chunkNumber;
            metadata["totalChunks"] = totalChunks;

            QJsonObject jsonObj;
            if (chatMessage.type == ChatMessage::Image) {
                jsonObj["type"] = "image";
            }
            else if (chatMessage.type == ChatMessage::File) {
                jsonObj["type"] = "file";
            }
            jsonObj["senderId"] = chatMessage.senderID;
            jsonObj["receiverId"] = chatMessage.receiverID;
            jsonObj["time"] = chatMessage.timestamp.toString();
            jsonObj["metadata"] = metadata;
            jsonObj["chunk"] = QString::fromUtf8(chunk.toBase64()); // Base64 编码

            sendMessage(jsonObj);

            offset += CHUNK_SIZE;
            chunkNumber++;
        }
        file.close();
    }
}

void ChatNetworkManager::sendSelfID() {
    if(mSelfID.isEmpty()) {
        return;
    }

    QJsonObject jsonObj;
    jsonObj["type"] = "clientId";
    jsonObj["value"] = mSelfID;

    sendMessage(jsonObj);
}

void ChatNetworkManager::onBinaryMessageReceived(const QByteArray &data) {

    ChatMessage chatMessage;

    // 解析 JSON 消息
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj["type"] == "text") {
        QString family = jsonObj["font"].toObject()["family"].toString();
        int pointSize = jsonObj["font"].toObject()["pointSize"].toInt();
        int weight = jsonObj["font"].toObject()["weight"].toInt();
        QColor fontColor = QColor(jsonObj["font"].toObject()["color"].toString());

        chatMessage.type = ChatMessage::Text;
        chatMessage.senderID = jsonObj["senderId"].toString();
        chatMessage.receiverID = jsonObj["receiverId"].toString();
        chatMessage.timestamp = QDateTime::fromString(jsonObj["time"].toString());
        chatMessage.content = jsonObj["content"].toString();
        chatMessage.font = QFont(family, pointSize, weight);
        chatMessage.fontColor = fontColor;
    }
    else if (jsonObj["type"] == "image" || jsonObj["type"] == "file") {
        if (jsonObj["type"] == "image") {
            chatMessage.type = ChatMessage::Image;
        }
        else if (jsonObj["type"] == "file") {
            chatMessage.type = ChatMessage::File;
        }

        chatMessage.senderID = jsonObj["senderId"].toString();
        chatMessage.receiverID = jsonObj["receiverId"].toString();
        chatMessage.timestamp = QDateTime::fromString(jsonObj["time"].toString());
        chatMessage.content = jsonObj["fileUrl"].toString();
        chatMessage.receivedFileName = jsonObj["fileName"].toString();

        qDebug() << "File name: " << chatMessage.receivedFileName;
        qDebug() << "File url: " << chatMessage.content;
    }
    else {
        return;
    }

    emit messageReceived(chatMessage);
}

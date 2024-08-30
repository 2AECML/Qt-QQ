#include "homenetworkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

// 构造函数，初始化 HomeNetworkManager
HomeNetworkManager::HomeNetworkManager(QObject *parent)
    : BasicTcpNetworkManager("113.45.223.108", 8053, parent) {
    // 使用 BasicTcpNetworkManager 构造函数初始化，指定服务器地址和端口
}

// 发送用户列表请求
void HomeNetworkManager::sendUserListRequest() {
    QJsonObject json;
    json["type"] = "user_list";  // 设置请求类型为"user_list"

    QByteArray data = appendDelimiter(json);  // 将 JSON 对象转换为带分隔符的数据
    sendRequest(data);  // 发送请求
}

// 发送用户信息请求
void HomeNetworkManager::sendUserInfoRequest(const QString& accountID) {
    QJsonObject json;
    json["type"] = "user_info";  // 设置请求类型为"user_info"
    json["id"] = accountID;  // 设置请求参数为指定的账号ID

    QByteArray data = appendDelimiter(json);  // 将 JSON 对象转换为带分隔符的数据
    sendRequest(data);  // 发送请求
}

// 处理接收到的数据
void HomeNetworkManager::onReadyRead() {
    QByteArray data = mTcpSocket->readAll();  // 读取所有接收到的数据

    QList<QJsonObject> jsonObjList = extractDataToJsonList(data);  // 解析数据为 JSON 对象列表

    // 处理每个 JSON 对象
    for (auto& jsonObj : jsonObjList) {
        if (jsonObj["type"] == "user_list") {
            // 处理用户列表
            QJsonArray jsonArray = jsonObj["list"].toArray();  // 提取用户列表
            for (const QJsonValue& value : jsonArray) {
                QJsonObject jsonObject = value.toObject();  // 提取每个用户对象
                QString id = jsonObject["id"].toString();  // 获取用户ID
                QString nickname = jsonObject["nickname"].toString();  // 获取用户昵称
                emit userListItemResponse(id, nickname);  // 发射用户列表项响应信号
            }
        }
        else if (jsonObj["type"] == "user_info") {
            // 处理用户信息
            QString id = jsonObj["id"].toString();  // 获取用户ID
            QString nickname = jsonObj["nickname"].toString();  // 获取用户昵称
            emit selfInfoResponse(id, nickname);  // 发射用户信息响应信号
        }
    }
}

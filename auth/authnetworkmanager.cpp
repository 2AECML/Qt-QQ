#include "authnetworkmanager.h"
#include <QJsonObject>
#include <QByteArray>

// 构造函数，初始化父类 BasicTcpNetworkManager
AuthNetworkManager::AuthNetworkManager(QObject *parent)
    : BasicTcpNetworkManager("113.45.223.108", 8052, parent) { // 设置服务器 IP 和端口
}

// 发送登录请求
void AuthNetworkManager::sendLoginRequest(const QString &account, const QString &password) {
    QJsonObject jsonObj; // 创建 JSON 对象
    jsonObj["type"] = "login"; // 设置请求类型为登录
    jsonObj["account"] = account; // 设置账户名
    jsonObj["password"] = password; // 设置密码

    QByteArray data = appendDelimiter(jsonObj); // 将 JSON 对象转化为字节数组并添加分隔符

    sendRequest(data); // 发送请求数据
}

// 发送注册请求
void AuthNetworkManager::sendRegisterRequest(const QString &nickname, const QString &password, const QString &phone, const QString &vcode) {
    QJsonObject jsonObj; // 创建 JSON 对象
    jsonObj["type"] = "register"; // 设置请求类型为注册
    jsonObj["nickname"] = nickname; // 设置昵称
    jsonObj["password"] = password; // 设置密码
    jsonObj["phone"] = phone; // 设置电话
    jsonObj["vcode"] = vcode; // 设置验证码

    QByteArray data = appendDelimiter(jsonObj); // 将 JSON 对象转化为字节数组并添加分隔符

    sendRequest(data); // 发送请求数据
}

// 处理从服务器读取的数据
void AuthNetworkManager::onReadyRead() {
    QByteArray data = mTcpSocket->readAll(); // 读取所有可用数据

    QList<QJsonObject> jsonObjList = extractDataToJsonList(data); // 将数据提取为 JSON 对象列表

    for (auto& jsonObj : jsonObjList) { // 遍历所有 JSON 对象
        if (jsonObj["type"] == "login") { // 如果是登录响应
            bool success = jsonObj["success"].toBool(); // 获取登录成功标志
            QString message = jsonObj["message"].toString(); // 获取响应消息
            QString id = jsonObj["id"].toString(); // 获取用户 ID

            emit loginResponse(success, message, id); // 发出登录响应信号
        }
        else if (jsonObj["type"] == "register") { // 如果是注册响应
            bool success = jsonObj["success"].toBool(); // 获取注册成功标志
            QString message = jsonObj["message"].toString(); // 获取响应消息
            QString id = jsonObj["id"].toString(); // 获取用户 ID

            emit registerResponse(success, message, id); // 发出注册响应信号
        }
    }
}

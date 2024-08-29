#include "authnetworkmanager.h"
#include <QJsonObject>
#include <QByteArray>

AuthNetworkManager::AuthNetworkManager(QObject *parent)
    : BasicTcpNetworkManager("192.168.124.129", 8052, parent) {

}

void AuthNetworkManager::sendLoginRequest(const QString &account, const QString &password) {
    QJsonObject jsonObj;
    jsonObj["type"] = "login";
    jsonObj["account"] = account;
    jsonObj["password"] = password;

    QByteArray data = appendDelimiter(jsonObj);

    sendRequest(data);
}

void AuthNetworkManager::sendRegisterRequest(const QString &nickname, const QString &password, const QString &phone, const QString &vcode) {
    QJsonObject jsonObj;
    jsonObj["type"] = "register";
    jsonObj["nickname"] = nickname;
    jsonObj["password"] = password;
    jsonObj["phone"] = phone;
    jsonObj["vcode"] = vcode;

    QByteArray data = appendDelimiter(jsonObj);

    sendRequest(data);
}

void AuthNetworkManager::onReadyRead() {  

    QByteArray data = mTcpSocket->readAll();

    QList<QJsonObject> jsonObjList = extractDataToJsonList(data);

    for (auto& jsonObj : jsonObjList) {
        if (jsonObj["type"] == "login") {
            bool success = jsonObj["success"].toBool();
            QString message = jsonObj["message"].toString();
            QString id = jsonObj["id"].toString();

            emit loginResponse(success, message, id);
            // qDebug() << json["message"].toString();
        }
        else if (jsonObj["type"] == "register") {
            bool success = jsonObj["success"].toBool();
            QString message = jsonObj["message"].toString();
            QString id = jsonObj["id"].toString();

            emit registerResponse(success, message, id);
            // qDebug() << json["message"].toString();
        }
    }
}



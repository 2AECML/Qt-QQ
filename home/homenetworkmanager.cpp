#include "homenetworkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

HomeNetworkManager::HomeNetworkManager(QObject *parent)
    : BasicTcpNetworkManager("192.168.124.129", 8053, parent) {

}

void HomeNetworkManager::sendUserListRequest() {
    QJsonObject json;
    json["type"] = "user_list";

    QByteArray data = appendDelimiter(json);
    sendRequest(data);
}

void HomeNetworkManager::sendUserInfoRequest(const QString& accountID) {
    QJsonObject json;
    json["type"] = "user_info";
    json["id"] = accountID;

    QByteArray data = appendDelimiter(json);
    sendRequest(data);
}

void HomeNetworkManager::onReadyRead() {
    QByteArray data = mTcpSocket->readAll();

    QList<QJsonObject> jsonObjList = extractDataToJsonList(data);

    for (auto& jsonObj : jsonObjList) {
        if (jsonObj["type"] == "user_list") {
            QJsonArray jsonArray = jsonObj["list"].toArray();
            for (const QJsonValue& value : jsonArray) {
                QJsonObject jsonObject = value.toObject();
                QString id = jsonObject["id"].toString();
                QString nickname = jsonObject["nickname"].toString();
                emit userListItemResponse(id, nickname);
            }
        }
        else if (jsonObj["type"] == "user_info") {
            QString id = jsonObj["id"].toString();
            QString nickname = jsonObj["nickname"].toString();
            emit selfInfoResponse(id, nickname);
        }
    }
}

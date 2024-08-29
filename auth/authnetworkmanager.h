#ifndef AUTHNETWORKMANAGER_H
#define AUTHNETWORKMANAGER_H

#include "../custom_widgets/basictcpnetworkmanager.h"
#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class AuthNetworkManager : public BasicTcpNetworkManager
{
    Q_OBJECT
public:
    explicit AuthNetworkManager(QObject *parent = nullptr);

    void sendLoginRequest(const QString& account, const QString& password);

    void sendRegisterRequest(const QString& nickname, const QString& password, const QString& phone, const QString& vcode);

private slots:
    void onReadyRead() override;

signals:
    void loginResponse(bool success, const QString& message, const QString& accountID);

    void registerResponse(bool success, const QString& message, const QString& accountID);

};

#endif // AUTHNETWORKMANAGER_H

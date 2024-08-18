#ifndef AUTHNETWORKMANAGER_H
#define AUTHNETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class AuthNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit AuthNetworkManager(QObject *parent = nullptr);

    ~AuthNetworkManager();

    void sendLoginRequest(const QString& account, const QString& password);

    void sendRegisterRequest(const QString& nickname, const QString& password, const QString& phone, const QString& vcode);

    bool isConnected() const;

    void disconnect();

signals:
    void loginResponse(bool success, const QString& message);

    void registerResponse(bool success, const QString& message);

private slots:
    void onReadyRead();

    void onConnected();

    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* mTcpSocket;

    void sendRequest(const QJsonObject& jsonData);

    void connectToServer();

};

#endif // AUTHNETWORKMANAGER_H

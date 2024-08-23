#ifndef HOMENETWORKMANAGER_H
#define HOMENETWORKMANAGER_H

#include <QTcpSocket>
#include <QObject>
#include <QAbstractSocket>

class HomeNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit HomeNetworkManager(QObject *parent = nullptr);
    void sendUserListRequest();
    void sendUserInfoRequest(const QString& accountID);

private:
    void sendRequest(const QJsonObject& jsonData);
    void connectToServer();

private slots:
    void onReadyRead();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* mTcpSocket;
    QString mHostName;
    qint16 mPort;

signals:
    void userListResponse(const QJsonArray& list);
    void userListItemResponse(const QString& id, const QString& nickname);
};

#endif // HOMENETWORKMANAGER_H

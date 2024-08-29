#ifndef BASICWEBNETWORKMANAGER_H
#define BASICWEBNETWORKMANAGER_H

#include <QObject>
#include <QWebSocket>
#include <QString>
#include <QByteArray>
#include <QUrl>
#include <QJsonObject>

class BasicWebNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit BasicWebNetworkManager(QUrl url, QObject *parent = nullptr);
    ~BasicWebNetworkManager();

    bool isConnected() const;
    void disconnectFromServer();

protected:
    void connectToServer();
    void sendMessage(const QString& message);
    void sendMessage(const QJsonObject& jsonObj);
    void sendMessage(const QByteArray& data);

protected slots:
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onTextMessageReceived(const QString& message);
    virtual void onBinaryMessageReceived(const QByteArray& data);
    virtual void onErrorOccurred(QAbstractSocket::SocketError socketError);

protected:
    QWebSocket* mWebSocket;
    QUrl mUrl;

signals:
};

#endif // BASICWEBNETWORKMANAGER_H

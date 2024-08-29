#ifndef BASICTCPNETWORKMANAGER_H
#define BASICTCPNETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QList>

class BasicTcpNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit BasicTcpNetworkManager(const QString& hostName, const qint64 port, QObject* parent = nullptr);
    ~BasicTcpNetworkManager();

    bool isConnected() const;
    void disconnectFromServer();

protected:
    void connectToServer();
    void sendRequest(const QJsonObject& jsonObj);
    void sendRequest(const QByteArray& data);
    QByteArray appendDelimiter(const QJsonObject& jsonObj);
    QList<QJsonObject> extractDataToJsonList(const QByteArray& data);

protected slots:
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onReadyRead();
    virtual void onErrorOccurred(QAbstractSocket::SocketError socketError);

protected:
    QTcpSocket* mTcpSocket;
    QString mHostName;
    qint16 mPort;

signals:
};

#endif // BASICTCPNETWORKMANAGER_H

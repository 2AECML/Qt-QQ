#ifndef CHATNETWORKMANAGER_H
#define CHATNETWORKMANAGER_H

#define CHUNK_SIZE 262144 // 256KB

#include "chatmessage.h"
#include "../custom_widgets/basicwebnetworkmanager.h"
#include <QByteArray>

class ChatNetworkManager : public BasicWebNetworkManager
{
    Q_OBJECT
public:
    explicit ChatNetworkManager(const QString& selfID, QObject* parent = nullptr);

    void sendChatMessage(const ChatMessage& chatMessage);

private:
    void sendSelfID();

private slots:
    void onBinaryMessageReceived(const QByteArray& data) override;

private:
    QString mSelfID;

signals:
    void messageReceived(const ChatMessage& chatMessage);
};

#endif // CHATNETWORKMANAGER_H

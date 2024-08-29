#ifndef CHATDATABASEMANAGER_H
#define CHATDATABASEMANAGER_H

#include "chatmessage.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>

class ChatDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatDatabaseManager(const QString& selfID, QObject* parent = nullptr);

    QList<ChatMessage> getChatHistory(const QString& otherID) const;

    void saveChatHistory(const ChatMessage& chatMessage);

private:
    void setupDatabase();

private:
    QString mSelfID;

signals:
};

#endif // CHATDATABASEMANAGER_H

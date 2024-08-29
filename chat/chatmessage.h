#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QDateTime>
#include <QFont>
#include <QColor>
#include <QFile>

struct ChatMessage {
    enum Type { Text, Image, File };

    Type type;
    QString senderID;
    QString receiverID;
    QString content;
    QDateTime timestamp;
    QFont font;
    QColor fontColor;
    QString receivedFileName;
};

#endif // CHATMESSAGE_H

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QDateTime>

struct ChatMessage {
    // enum Type { Text, Image, File };

    // Type type;
    QString senderID;
    QString receiverID;
    QString content;
    QDateTime timestamp;

};

#endif // CHATMESSAGE_H

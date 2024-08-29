#include "chatdatabasemanager.h"

ChatDatabaseManager::ChatDatabaseManager(const QString& selfID, QObject *parent)
    : mSelfID(selfID)
    , QObject(parent){

    setupDatabase();
}

QList<ChatMessage> ChatDatabaseManager::getChatHistory(const QString &otherID) const{
    QList<ChatMessage> result;
    QSqlQuery query;
    QString tableName = QString("messages_%1").arg(mSelfID);

    query.prepare(QString("SELECT senderID, receiverID, content, timestamp, type, fontFamily, fontSize, fontWeight, fontColor, receivedFileName FROM %1 "
                          "WHERE (senderID = :selfID AND receiverID = :otherID) OR (senderID = :otherID AND receiverID = :selfID) "
                          "ORDER BY timestamp ASC").arg(tableName));
    query.bindValue(":selfID", mSelfID);
    query.bindValue(":otherID", otherID);

    if (!query.exec()) {
        qWarning() << "Failed to retrieve chat history:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        ChatMessage chatMessage;
        chatMessage.senderID = query.value("senderID").toString();
        chatMessage.receiverID = query.value("receiverID").toString();
        chatMessage.content = query.value("content").toString();
        chatMessage.timestamp = query.value("timestamp").toDateTime();
        chatMessage.type = static_cast<ChatMessage::Type>(query.value("type").toInt());
        chatMessage.font.setFamily(query.value("fontFamily").toString());
        chatMessage.font.setPointSize(query.value("fontSize").toInt());
        chatMessage.font.setWeight(static_cast<QFont::Weight>(query.value("fontWeight").toInt()));
        chatMessage.fontColor = QColor(query.value("fontColor").toString());
        chatMessage.receivedFileName = query.value("receivedFileName").toString();

        result.append(chatMessage);
    }

    return result;
}

void ChatDatabaseManager::saveChatHistory(const ChatMessage &chatMessage) {
    QSqlQuery query;
    QString tableName = QString("messages_%1").arg(mSelfID);

    query.prepare(QString("INSERT INTO %1 (senderID, receiverID, content, timestamp, type, fontFamily, fontSize, fontWeight, fontColor, receivedFileName) "
                          "VALUES (:senderID, :receiverID, :content, :timestamp, :type, :fontFamily, :fontSize, :fontWeight, :fontColor, :receivedFileName)")
                      .arg(tableName));
    query.bindValue(":senderID", chatMessage.senderID);
    query.bindValue(":receiverID", chatMessage.receiverID);
    query.bindValue(":content", chatMessage.content);
    query.bindValue(":timestamp", chatMessage.timestamp);
    query.bindValue(":type", chatMessage.type);
    query.bindValue(":fontFamily", chatMessage.font.family());
    query.bindValue(":fontSize", chatMessage.font.pointSize());
    query.bindValue(":fontWeight", chatMessage.font.weight());
    query.bindValue(":fontColor", chatMessage.fontColor.name(QColor::QColor::HexRgb));
    query.bindValue(":receivedFileName", chatMessage.receivedFileName);

    if (!query.exec()) {
        qWarning() << "Failed to insert message:" << query.lastError().text();
    }
}

void ChatDatabaseManager::setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat_history.db");

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS user_tables (userID TEXT PRIMARY KEY)")) {
        qWarning() << "Failed to create user_tables table:" << query.lastError().text();
    }


    QString tableName = QString("messages_%1").arg(mSelfID);

    if (!query.exec(QString("CREATE TABLE IF NOT EXISTS %1 ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "senderID TEXT, "
                            "receiverID TEXT, "
                            "content TEXT, "
                            "timestamp DATETIME, "
                            "type INTEGER, "
                            "fontFamily TEXT, "
                            "fontSize INTEGER, "
                            "fontWeight INTEGER, "
                            "fontColor TEXT, "
                            "receivedFileName TEXT)").arg(tableName))) {
        qWarning() << "Failed to create user table:" << query.lastError().text();
    }
}

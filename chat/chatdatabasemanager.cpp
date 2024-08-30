#include "chatdatabasemanager.h"

// 构造函数，初始化自定义ID并设置数据库
ChatDatabaseManager::ChatDatabaseManager(const QString& selfID, QObject *parent)
    : mSelfID(selfID) // 初始化用户ID
    , QObject(parent) { // 调用基类构造函数
    setupDatabase(); // 设置数据库
}

// 获取与另一个用户的聊天记录
QList<ChatMessage> ChatDatabaseManager::getChatHistory(const QString &otherID) const {
    QList<ChatMessage> result; // 存储聊天记录
    QSqlQuery query; // 数据库查询对象
    QString tableName = QString("messages_%1").arg(mSelfID); // 构造表名

    // 准备 SQL 查询语句
    query.prepare(QString("SELECT senderID, receiverID, content, timestamp, type, fontFamily, fontSize, fontWeight, fontColor, receivedFileName FROM %1 "
                          "WHERE (senderID = :selfID AND receiverID = :otherID) OR (senderID = :otherID AND receiverID = :selfID) "
                          "ORDER BY timestamp ASC").arg(tableName));
    query.bindValue(":selfID", mSelfID); // 绑定参数 selfID
    query.bindValue(":otherID", otherID); // 绑定参数 otherID

    if (!query.exec()) { // 执行查询
        qWarning() << "Failed to retrieve chat history:" << query.lastError().text(); // 查询失败日志
        return result; // 返回空结果
    }

    // 处理查询结果
    while (query.next()) {
        ChatMessage chatMessage; // 创建聊天消息对象
        chatMessage.senderID = query.value("senderID").toString(); // 设置发送者ID
        chatMessage.receiverID = query.value("receiverID").toString(); // 设置接收者ID
        chatMessage.content = query.value("content").toString(); // 设置消息内容
        chatMessage.timestamp = query.value("timestamp").toDateTime(); // 设置时间戳
        chatMessage.type = static_cast<ChatMessage::Type>(query.value("type").toInt()); // 设置消息类型
        chatMessage.font.setFamily(query.value("fontFamily").toString()); // 设置字体
        chatMessage.font.setPointSize(query.value("fontSize").toInt()); // 设置字体大小
        chatMessage.font.setWeight(static_cast<QFont::Weight>(query.value("fontWeight").toInt())); // 设置字体粗细
        chatMessage.fontColor = QColor(query.value("fontColor").toString()); // 设置字体颜色
        chatMessage.receivedFileName = query.value("receivedFileName").toString(); // 设置接收的文件名

        result.append(chatMessage); // 添加到结果列表
    }

    return result; // 返回聊天记录
}

// 保存聊天记录到数据库
void ChatDatabaseManager::saveChatHistory(const ChatMessage &chatMessage) {
    QSqlQuery query; // 数据库查询对象
    QString tableName = QString("messages_%1").arg(mSelfID); // 构造表名

    // 准备 SQL 插入语句
    query.prepare(QString("INSERT INTO %1 (senderID, receiverID, content, timestamp, type, fontFamily, fontSize, fontWeight, fontColor, receivedFileName) "
                          "VALUES (:senderID, :receiverID, :content, :timestamp, :type, :fontFamily, :fontSize, :fontWeight, :fontColor, :receivedFileName)")
                      .arg(tableName));
    query.bindValue(":senderID", chatMessage.senderID); // 绑定参数 senderID
    query.bindValue(":receiverID", chatMessage.receiverID); // 绑定参数 receiverID
    query.bindValue(":content", chatMessage.content); // 绑定参数 content
    query.bindValue(":timestamp", chatMessage.timestamp); // 绑定参数 timestamp
    query.bindValue(":type", chatMessage.type); // 绑定参数 type
    query.bindValue(":fontFamily", chatMessage.font.family()); // 绑定参数 fontFamily
    query.bindValue(":fontSize", chatMessage.font.pointSize()); // 绑定参数 fontSize
    query.bindValue(":fontWeight", chatMessage.font.weight()); // 绑定参数 fontWeight
    query.bindValue(":fontColor", chatMessage.fontColor.name(QColor::HexRgb)); // 绑定参数 fontColor
    query.bindValue(":receivedFileName", chatMessage.receivedFileName); // 绑定参数 receivedFileName

    if (!query.exec()) { // 执行插入操作
        qWarning() << "Failed to insert message:" << query.lastError().text(); // 插入失败日志
    }
}

// 设置数据库，包括创建表
void ChatDatabaseManager::setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // 添加 SQLite 数据库
    db.setDatabaseName("chat_history.db"); // 设置数据库文件名

    if (!db.open()) { // 打开数据库
        qWarning() << "Failed to open database:" << db.lastError().text(); // 打开失败日志
        return; // 返回
    }

    QSqlQuery query; // 数据库查询对象
    if (!query.exec("CREATE TABLE IF NOT EXISTS user_tables (userID TEXT PRIMARY KEY)")) { // 创建用户表
        qWarning() << "Failed to create user_tables table:" << query.lastError().text(); // 创建失败日志
    }

    QString tableName = QString("messages_%1").arg(mSelfID); // 构造聊天记录表名

    // 创建聊天记录表
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
        qWarning() << "Failed to create user table:" << query.lastError().text(); // 创建失败日志
    }
}

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "../custom_widgets/basicwidget.h"
#include "chatmessage.h"
#include "chatnetworkmanager.h"
#include "chatdatabasemanager.h"
#include <QCloseEvent>
#include <QListWidget>
#include <QFont>
#include <QColor>
#include <QDateTime>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public BasicWidget {
    Q_OBJECT

public:
    explicit ChatWindow(const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName, QWidget* parent = nullptr);
    ~ChatWindow();

    void setupConnection();

protected:
    void loadStyle();
    void closeEvent(QCloseEvent* event);

private:
    void loadHistory();
    void sendMessage(const QString& message, const ChatMessage::Type type = ChatMessage::Text);
    void appendMessageToChat(const ChatMessage& message, bool isHistory = false);
    QString downloadImage(const QString &urlString);

private:
    Ui::ChatWindow* ui;

    ChatNetworkManager* mNetworkManager;
    ChatDatabaseManager* mDatabaseManager;

    QString mSelfID;
    QString mOtherID;
    QString mSelfName;
    QString mOtherName;

    QFont mFont;
    QColor mFontColor;

    QDateTime mLastSendTime;

    const QString mLocalImagePath = "file cache";

signals:
    void closed();
};

#endif // CHATWINDOW_H

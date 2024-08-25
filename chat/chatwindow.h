#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "../custom_widgets/customwidget.h"
#include "chatmessage.h"
#include <QCloseEvent>
#include <QListWidget>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public CustomWidget {
    Q_OBJECT

public:
    explicit ChatWindow(const QString& selfID, const QString& otherID, QWidget* parent = nullptr);
    ~ChatWindow();

    void setupConnection();

protected:
    void loadStyle();

private:
    void sendMessage(const QString &messageContent);
    void appendMessageToChat(const ChatMessage& message);

private:
    Ui::ChatWindow* ui;

    QString mSelfID;
    QString mOtherID;

signals:

};

#endif // CHATWINDOW_H

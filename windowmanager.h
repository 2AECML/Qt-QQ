#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include "auth/authwindow.h"
#include "home/homewindow.h"
#include "chat/chatwindow.h"

class WindowManager : public QObject {
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = nullptr);

private:
    AuthWindow* mAuthWindow;
    HomeWindow* mHomeWindow;
    ChatWindow* mChatWindow;
};

#endif // WINDOWMANAGER_H

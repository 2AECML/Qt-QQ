#include "windowmanager.h"
#include "auth/authwindow.h"
#include "home/homewindow.h"
#include "chat/chatwindow.h"
#include <QString>
#include <QScopedPointer>

WindowManager::WindowManager(QObject *parent)
    : QObject(parent) {

    // handleChatWindowRequested("10005", "10001", "呜呜呜", "测试用户");

    auto authWindow = new AuthWindow();
    authWindow->show();

    connect(authWindow, &AuthWindow::closed, this, [authWindow]() {
        authWindow->deleteLater();
    });

    connect(authWindow, &AuthWindow::loginSuccessful, this, [this](const QString& accountID) {
        handleLoginSuccessful(accountID);
    });
}

void WindowManager::handleLoginSuccessful(const QString& accountID) {
    auto homeWindow = new HomeWindow(accountID);
    homeWindow->show();

    connect(homeWindow, &HomeWindow::closed, this, [homeWindow]() {
        homeWindow->deleteLater();
    });

    connect(homeWindow, &HomeWindow::chatWindowRequested, this,
            [this](const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName) {
        handleChatWindowRequested(selfID, otherID, selfName, otherName);
    });
}

void WindowManager::handleChatWindowRequested(const QString& selfID, const QString& otherID, const QString &selfName, const QString& otherName) {
    auto chatWindow = new ChatWindow(selfID, otherID, selfName, otherName);
    chatWindow->show();

    connect(chatWindow, &ChatWindow::closed, this, [chatWindow]() {
        chatWindow->deleteLater();
    });
}

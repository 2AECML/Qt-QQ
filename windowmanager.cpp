#include "windowmanager.h"
#include "auth/authwindow.h"
#include "home/homewindow.h"
#include "chat/chatwindow.h"
#include <QString>
#include <QScopedPointer>

// 构造函数
WindowManager::WindowManager(QObject *parent)
    : QObject(parent) {

    // 创建并显示认证窗口
    auto authWindow = new AuthWindow();
    authWindow->show();

    // 连接认证窗口的关闭信号到删除该窗口的槽函数
    connect(authWindow, &AuthWindow::closed, this, [authWindow]() {
        authWindow->deleteLater();
    });

    // 连接认证成功信号到处理登录成功的槽函数
    connect(authWindow, &AuthWindow::loginSuccessful, this, [this](const QString& accountID) {
        handleLoginSuccessful(accountID);
    });
}

// 处理登录成功的槽函数
void WindowManager::handleLoginSuccessful(const QString& accountID) {
    // 创建并显示主页窗口
    auto homeWindow = new HomeWindow(accountID);
    homeWindow->show();

    // 连接主页窗口的关闭信号到删除该窗口的槽函数
    connect(homeWindow, &HomeWindow::closed, this, [homeWindow]() {
        homeWindow->deleteLater();
    });

    // 连接请求聊天窗口信号到处理聊天窗口请求的槽函数
    connect(homeWindow, &HomeWindow::chatWindowRequested, this,
            [this](const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName) {
                handleChatWindowRequested(selfID, otherID, selfName, otherName);
            });
}

// 处理聊天窗口请求的槽函数
void WindowManager::handleChatWindowRequested(const QString& selfID, const QString& otherID, const QString &selfName, const QString& otherName) {
    // 创建并显示聊天窗口
    auto chatWindow = new ChatWindow(selfID, otherID, selfName, otherName);
    chatWindow->show();

    // 连接聊天窗口的关闭信号到删除该窗口的槽函数
    connect(chatWindow, &ChatWindow::closed, this, [chatWindow]() {
        chatWindow->deleteLater();
    });
}

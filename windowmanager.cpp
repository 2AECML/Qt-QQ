#include "windowmanager.h"
#include <QString>

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
    , mAuthWindow(nullptr)
    , mHomeWindow(nullptr)
    , mChatWindow(nullptr) {

    // mAuthWindow = new AuthWindow();
    // mAuthWindow->show();

    // connect(mAuthWindow, &AuthWindow::closed, this, [this]() {
    //     mAuthWindow->deleteLater();
    // });

    // connect(mAuthWindow, &AuthWindow::loginSuccessful, this, [this](const QString& accountID) {
    //     mHomeWindow = new HomeWindow(accountID);
    //     mHomeWindow->show();
    //     connect(mHomeWindow, &HomeWindow::closed, this, [this]() {
    //         mHomeWindow->deleteLater();
    //     });

    //     connect(mHomeWindow, &HomeWindow::chatWindowRequested, this, [this](const QString& selfID, const QString& otherID) {
            mChatWindow = new ChatWindow("10005", "10001");
            mChatWindow->show();
    //     });
    // });

}

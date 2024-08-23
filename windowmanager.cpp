#include "windowmanager.h"
#include <QString>

WindowManager::WindowManager(QObject *parent) {
    mAuthWindow = new AuthWindow();
    mAuthWindow->show();

    connect(mAuthWindow, &AuthWindow::closed, this, [this]() {
        mAuthWindow->deleteLater();
    });

    connect(mAuthWindow, &AuthWindow::loginSuccessful, this, [this](const QString& accountID) {
        mHomeWindow = new HomeWindow(accountID);
        mHomeWindow->show();
        connect(mHomeWindow, &HomeWindow::closed, this, [this]() {
            mHomeWindow->deleteLater();
        });

        mAuthWindow->deleteLater();
    });

}

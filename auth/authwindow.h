#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include "authnetworkmanager.h"
#include "loginwidget.h"
#include "registerwidget.h"
#include "waitwidget.h"
#include "../custom_widgets/customwidget.h"
#include <QWidget>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QTimer>
#include <QLineEdit>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class AuthWindow;
}
QT_END_NAMESPACE

class AuthWindow : public CustomWidget {
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

protected:
    void closeEvent(QCloseEvent* event) override;
    void loadStyle();

private:
    void setupConnection();
    void initPages();
    void fadeOut(QWidget* targetPage) const;
    void fadeIn(QWidget* targetPage) const;
    void updateStyle(QWidget* w);

private slots:
    void onCloseBtnClicked();
    void onMinimizeBtnClicked();
    void goToRegisterPage();
    void goToLoginPage();
    void goToWaitPage(WaitWidget::Type waitType);
    void returnToLastPage();
    void onLoginResponse(bool success, const QString& message, const QString &accountID);
    void onRegisterResponse(bool success, const QString& message, const QString& accountID);
    void onCurrentChanged(int index);

private:
    Ui::AuthWindow *ui;

    LoginWidget* mLoginPage;

    RegisterWidget* mRegisterPage;

    WaitWidget* mWaitPage;

    int mLastPageIndex;

    AuthNetworkManager* mNetworkManager;

signals:
    void closed();
    void loginSuccessful(const QString& accountID);
};


#endif // AUTHWINDOW_H

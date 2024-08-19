#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include "authnetworkmanager.h"
#include "loginwidget.h"
#include "registerwidget.h"
#include "waitwidget.h"
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

class AuthWindow : public QWidget
{
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void installEventFilters();
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
    void onLoginResponse(bool success, const QString& message);
    void onRegisterResponse(bool success, const QString& message);
    void onCurrentChanged(int index);

private:
    Ui::AuthWindow *ui;

    QPoint mDragPosition;

    bool mDragEnabled = true;

    LoginWidget* mLoginPage;

    RegisterWidget* mRegisterPage;

    WaitWidget* mWaitPage;

    int mLastPageIndex;

    AuthNetworkManager* mNetworkManager;
};


#endif // AUTHWINDOW_H

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private:
    void setupConnection();
    void updateStyle(QWidget* w);

private slots:
    void onAgreeBtnClicked(bool checked);
    void onLoginBtnClicked();

private:
    Ui::LoginWidget *ui;
    bool mAgreeChecked = false;

signals:
    void loginSignal(const QString& account, const QString& password);
    void goToRegisterSignal();
};

#endif // LOGINWIDGET_H

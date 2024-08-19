#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QStyle>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget) {
    ui->setupUi(this);

    setupConnection();
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::setupConnection() {
    connect(ui->loginAgreeBtn, &QRadioButton::clicked, this, &LoginWidget::onAgreeBtnClicked);

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginBtnClicked);

    connect(ui->toRegisterBtn, &QPushButton::clicked, this, [this]() {
        emit goToRegisterSignal();
    });

}

void LoginWidget::updateStyle(QWidget *w) {
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

void LoginWidget::onAgreeBtnClicked(bool checked) {
    QPushButton* loginBtn = ui->loginBtn;

    mAgreeChecked = checked;

    if (checked) {
        loginBtn->setProperty("agreement", true);
        loginBtn->setCursor(Qt::PointingHandCursor);
    }
    else {
        loginBtn->setProperty("agreement", false);
        loginBtn->setCursor(Qt::ForbiddenCursor);
    }

    updateStyle(loginBtn);
}

void LoginWidget::onLoginBtnClicked() {
    if (mAgreeChecked == true) {
        const QString& account = ui->loginAccountInput->text();
        const QString& password = ui->loginPasswordInput->text();

        // qDebug() << "账号：" << account;
        // qDebug() << "密码：" << password;

        emit loginSignal(account, password);
    }
}

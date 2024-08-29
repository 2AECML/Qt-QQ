#include "authwindow.h"
#include "ui_authwindow.h"
#include "authhintdialog.h"
#include <QPropertyAnimation>
#include <QFile>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QString>
#include <QList>
#include <QLabel>

AuthWindow::AuthWindow(QWidget *parent)
    : BasicWidget(parent)
    , ui(new Ui::AuthWindow())
    , mLoginPage(new LoginWidget(ui->stackedWidget))
    , mRegisterPage(new RegisterWidget(ui->stackedWidget))
    , mWaitPage(new WaitWidget(ui->stackedWidget))
    , mNetworkManager(new AuthNetworkManager(this)) {

    ui->setupUi(this);

    initPages();

    setupConnection();

    loadStyle();
}

AuthWindow::~AuthWindow() {
    delete ui;
    qDebug() << "deleted AuthWindow";
}

void AuthWindow::closeEvent(QCloseEvent *event) {
    // 关闭网络连接
    if (mNetworkManager) {
        mNetworkManager->disconnectFromServer();
    }

    emit closed();

    event->accept();
}

void AuthWindow::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/auth.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
    }
}

void AuthWindow::setupConnection() {
    connect(ui->closeBtn, &QToolButton::clicked, this, &AuthWindow::onCloseBtnClicked);

    connect(ui->minimizeBtn, &QToolButton::clicked, this, &AuthWindow::onMinimizeBtnClicked);

    connect(mLoginPage, &LoginWidget::loginSignal, this, [this](const QString& account, const QString& password) {
        mNetworkManager->sendLoginRequest(account, password);
        goToWaitPage(WaitWidget::Type::Logining);
    });

    connect(mRegisterPage, &RegisterWidget::registerSignal, this, [this](const QString& nickname, const QString& password, const QString& phone, const QString& vcode) {
        mNetworkManager->sendRegisterRequest(nickname, password, phone, vcode);
        goToWaitPage(WaitWidget::Type::Registration);
    });

    connect(mLoginPage, &LoginWidget::goToRegisterSignal, this, &AuthWindow::goToRegisterPage);

    connect(mRegisterPage, &RegisterWidget::goToLoginSignal, this, &AuthWindow::goToLoginPage);

    connect(mNetworkManager, &AuthNetworkManager::loginResponse, this, &AuthWindow::onLoginResponse);

    connect(mNetworkManager, &AuthNetworkManager::registerResponse, this, &AuthWindow::onRegisterResponse);

    connect(mWaitPage, &WaitWidget::cancelSignal, this, [this]() {
        mNetworkManager->disconnectFromServer();
        returnToLastPage();
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &AuthWindow::onCurrentChanged);
}

void AuthWindow::initPages() {
    ui->stackedWidget->addWidget(mLoginPage);
    ui->stackedWidget->addWidget(mRegisterPage);
    ui->stackedWidget->addWidget(mWaitPage);

    ui->stackedWidget->setCurrentWidget(mLoginPage);

    mLastPageIndex = ui->stackedWidget->currentIndex();
}

void AuthWindow::fadeOut(QWidget* targetPage) const {
    if (!targetPage) {
        return;
    }

    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(targetPage);
    targetPage->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);

    connect(animation, &QPropertyAnimation::finished, this, [opacityEffect, animation]() {
        if (opacityEffect) {
            opacityEffect->deleteLater();
        }
        if (animation) {
            animation->deleteLater();
        }
    });

    animation->start();
}

void AuthWindow::fadeIn(QWidget *targetPage) const {
    if (!targetPage) {
        return;
    }

    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(targetPage);
    targetPage->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);

    QObject::connect(animation, &QPropertyAnimation::finished, this, [animation, opacityEffect]() {
        if (opacityEffect) {
            opacityEffect->deleteLater();
        }
        if (animation) {
            animation->deleteLater();
        }
    });

    animation->start();
}

void AuthWindow::updateStyle(QWidget *w) {
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

void AuthWindow::onCloseBtnClicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(100); // 动画持续时间
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->close();
        this->setWindowOpacity(1.0); // 恢复透明度
    });
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AuthWindow::onMinimizeBtnClicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200); // 动画持续时间
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->showMinimized();
        this->setWindowOpacity(1.0); // 恢复透明度
    });
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AuthWindow::goToRegisterPage() {
    mLastPageIndex = ui->stackedWidget->currentIndex();

    int targetIndex = ui->stackedWidget->indexOf(mRegisterPage);
    QWidget* targetPage = ui->stackedWidget->widget(targetIndex);

    ui->stackedWidget->setCurrentWidget(targetPage);
}

void AuthWindow::goToLoginPage() {
    mLastPageIndex = ui->stackedWidget->currentIndex();

    int targetIndex = ui->stackedWidget->indexOf(mLoginPage);
    QWidget* targetPage = ui->stackedWidget->widget(targetIndex);

    ui->stackedWidget->setCurrentWidget(targetPage);
}

void AuthWindow::goToWaitPage(WaitWidget::Type waitType) {
    mLastPageIndex = ui->stackedWidget->currentIndex();

    mWaitPage->setType(waitType);

    int targetIndex = ui->stackedWidget->indexOf(mWaitPage);
    QWidget* targetPage = ui->stackedWidget->widget(targetIndex);

    ui->stackedWidget->setCurrentWidget(targetPage);
}

void AuthWindow::returnToLastPage() {
    ui->stackedWidget->setCurrentIndex(mLastPageIndex);
}

void AuthWindow::onLoginResponse(bool success, const QString &message, const QString& accountID) {
    if (success) {
        qDebug() << "Login successful:" << message;
        // 处理登录成功逻辑
        AuthHintDialog dialog(this);
        dialog.setHint(message); 
        emit loginSuccessful(accountID);
        this->close();
    } else {
        qDebug() << "Login failed:" << message;
        // 处理登录失败逻辑
        AuthHintDialog dialog;
        dialog.setHint(message);
        connect(&dialog, &QDialog::finished, this, [this, &dialog]() {
            mNetworkManager->disconnectFromServer();
            dialog.close();
            returnToLastPage();
        });
        dialog.exec();
    }
}

void AuthWindow::onRegisterResponse(bool success, const QString& message, const QString& accountID) {
    if (success) {
        qDebug() << "Register successful:" << message;
        // 处理注册成功逻辑
        AuthHintDialog dialog;
        dialog.setHint(message, accountID);
        connect(&dialog, &QDialog::finished, this, [this, &dialog]() {
            mNetworkManager->disconnectFromServer();
            dialog.close();
            returnToLastPage();
        });
        dialog.exec();
    } else {
        qDebug() << "Register failed:" << message;
        // 处理注册失败逻辑
        AuthHintDialog dialog;
        dialog.setHint(message);
        connect(&dialog, &QDialog::finished, this, [this, &dialog]() {
            mNetworkManager->disconnectFromServer();
            dialog.close();
            returnToLastPage();
        });
        dialog.exec();
    }
}

void AuthWindow::onCurrentChanged(int index) {
    QWidget* currentPage = ui->stackedWidget->widget(index);
    if (currentPage) {
        fadeIn(currentPage);
    }
}



#include "authwindow.h"
#include "ui_authwindow.h"
#include "inputvalidator.h"
#include <QPropertyAnimation>
#include <QFile>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QString>
#include <QList>
#include <QLabel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

AuthWindow::AuthWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthWindow())
    , mVcodeTimer(new QTimer(this))
    , mCountdownNum(60)
    , mNetworkManager(new AuthNetworkManager(this)){

    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);

    setupConnection();

    installEventFilters();

    loadStyle();
}

AuthWindow::~AuthWindow() {
    delete ui;
}

void AuthWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (mDragEnabled) {
            mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    } else {
        QWidget::mousePressEvent(event); // 调用基类处理其他按键事件
    }
}

void AuthWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && mDragEnabled) {
        QPoint newPos = event->globalPosition().toPoint() - mDragPosition;
        move(newPos);
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event); // 调用基类处理其他移动事件
    }
}

void AuthWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mDragEnabled = true; // 鼠标释放，重新启用拖动
        event->accept();
    } else {
        QWidget::mouseReleaseEvent(event); // 调用基类处理其他释放事件
    }
}

bool AuthWindow::eventFilter(QObject *obj, QEvent *event) {
    // 获取按钮
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);

    if (event->type() == QEvent::MouseButtonPress) {
        if (button) {
            mDragEnabled = false; // 点击按钮，禁用拖动
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        if (button) {
            mDragEnabled = true;
        }
    }
    // 继续事件处理链
    return QWidget::eventFilter(obj, event);
}

void AuthWindow::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void AuthWindow::closeEvent(QCloseEvent *event) {
    // 关闭网络连接
    if (mNetworkManager) {
        mNetworkManager->disconnect();
    }

    event->accept();
}

void AuthWindow::installEventFilters() {
    // 遍历 UI 中的所有按钮并安装事件过滤器
    QList<QAbstractButton*> buttons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton* button : buttons) {
        button->installEventFilter(this);
    }
}

void AuthWindow::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/qss/auth.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
    }
}

void AuthWindow::setupConnection() const {
    connect(ui->closeBtn, &QToolButton::clicked, this, &AuthWindow::onCloseBtnClicked);

    connect(ui->minimizeBtn, &QToolButton::clicked, this, &AuthWindow::onMinimizeBtnClicked);

    connect(ui->loginAgreeBtn, &QRadioButton::clicked, this, &AuthWindow::onAgreeBtnClicked);

    connect(ui->loginBtn, &QPushButton::clicked, this, &AuthWindow::onLoginBtnClicked);

    connect(ui->registerBtn, &QPushButton::clicked, this, &AuthWindow::onRegisterBtnClicked);

    connect(ui->toRegisterBtn, &QPushButton::clicked, this, &AuthWindow::goToRegisterPage);

    connect(ui->toLoginBtn, &QPushButton::clicked, this, &AuthWindow::goToLoginPage);

    connect(ui->registerVcodeBtn, &QPushButton::clicked, this, &AuthWindow::onVcodeBtnClicked);

    connect(mVcodeTimer, &QTimer::timeout, this, &AuthWindow::updateVcodeBtnText);

    // 遍历所有QLineEdit对象
    QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
    for (QLineEdit *lineEdit : lineEdits) {
        connect(lineEdit, &QLineEdit::editingFinished, this, &AuthWindow::onEditingFinished);
    }

    connect(mNetworkManager, &AuthNetworkManager::loginResponse, this, &AuthWindow::onLoginResponse);

    connect(mNetworkManager, &AuthNetworkManager::registerResponse, this, &AuthWindow::onRegisterResponse);
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

void AuthWindow::updateVcodeBtnText() {
    QPushButton* vcodeBtn = ui->registerVcodeBtn;

    if (mCountdownNum > 0) {
        vcodeBtn->setText(QString("重新发送(%1)").arg(mCountdownNum--));
    } else {
        vcodeBtn->setText("发送验证码");
        vcodeBtn->setProperty("waiting", false);
        vcodeBtn->setEnabled(true);
        mVcodeTimer->stop();
        mCountdownNum = 60;
    }
    // 强制重新应用样式
    updateStyle(vcodeBtn);
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

void AuthWindow::onAgreeBtnClicked(bool checked)
{
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

void AuthWindow::onVcodeBtnClicked() {
    bool validPhone = ui->registerPhoneInput->property("valid").toBool();

    if (!validPhone) {
        return;
    }

    QPushButton* vcodeBtn = ui->registerVcodeBtn;

    if (!vcodeBtn->property("waiting").toBool()) {
        vcodeBtn->setProperty("waiting", true);
        vcodeBtn->setEnabled(false);
        mVcodeTimer->start(1000);
        updateVcodeBtnText();
    }

    updateStyle(vcodeBtn);
}

void AuthWindow::onLoginBtnClicked() {
    if (mAgreeChecked == true) {
        const QString& account = ui->loginAccountInput->text();
        const QString& password = ui->loginPasswordInput->text();

        // qDebug() << "账号：" << account;
        // qDebug() << "密码：" << password;

        mNetworkManager->sendLoginRequest(account, password);
    }
}

void AuthWindow::onRegisterBtnClicked() {
    const QString& nickname = ui->registerNicknameInput->text();
    const QString& password = ui->registerPasswordInput->text();
    const QString& phone = ui->registerPhoneInput->text();
    const QString& vcode = ui->registerVcodeInput->text();

    // qDebug() << "昵称：" << nickname;
    // qDebug() << "密码：" << password;
    // qDebug() << "手机号码：" << phone;
    // qDebug() << "验证码：" << vcode;

    bool nicknameValid = InputValidator::isNicknameValid(nickname);
    bool passwordValid = InputValidator::isPasswordValid(password);
    bool phoneValid = InputValidator::isPhoneValid(phone);

    // nicknameValid ? qDebug() << "昵称合法" : qDebug() << "昵称不合法";
    // passwordValid ? qDebug() << "密码合法" : qDebug() << "密码不合法";
    // phoneValid ? qDebug() << "手机号码合法" : qDebug() << "手机号码不合法";

    if (nicknameValid && passwordValid && phoneValid) {
        mNetworkManager->sendRegisterRequest(nickname, password, phone, vcode);
    }
}

void AuthWindow::onEditingFinished() {
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());

    if (!lineEdit) {
        return;
    }

    QString lineName = lineEdit->objectName();

    if (lineName == "registerNicknameInput") {
        QLabel* hint = ui->nicknameHint;

        if (lineEdit->text().isEmpty()) {
            hint->setText("昵称不能为空");
            hint->setProperty("valid", false);
            lineEdit->setProperty("valid", false);
        }
        else {
            bool valid = InputValidator::isNicknameValid(lineEdit->text());

            if (valid) {
                hint->setText("昵称合法");
                hint->setProperty("valid", true);
                lineEdit->setProperty("valid", true);
            }
            else {
                hint->setText("24位内任意字符，不允许全为空格");
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }

        updateStyle(hint);
    }
    else if (lineName == "registerPasswordInput") {
        QLabel* hint = ui->passwordHint;

        if (lineEdit->text().isEmpty()) {
            hint->setText("密码不能为空");
            hint->setProperty("valid", false);
            lineEdit->setProperty("valid", false);
        }
        else {
            bool valid = InputValidator::isPasswordValid(lineEdit->text());

            if (valid) {
                hint->setText("密码合法");
                hint->setProperty("valid", true);
                lineEdit->setProperty("valid", true);
            }
            else {
                hint->setText("8-16个字符，不允许出现空格或全为数字");
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }

        updateStyle(hint);
    }
    else if (lineName == "registerPhoneInput") {
        QLabel* hint = ui->phoneHint;

        if (lineEdit->text().isEmpty()) {
            hint->setText("手机号码不能为空");
            hint->setProperty("valid", false);
            lineEdit->setProperty("valid", false);
        }
        else {
            bool valid = InputValidator::isPhoneValid(lineEdit->text());

            if (valid) {
                hint->setText("手机号码合法");
                hint->setProperty("valid", true);
                lineEdit->setProperty("valid", true);
            }
            else {
                hint->setText("11位手机号码");
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }

        updateStyle(hint);
    }
    else {
        return;
    }

    updateStyle(lineEdit);
}

void AuthWindow::goToRegisterPage() {
    QWidget* targetPage = ui->stackedWidget->widget(1);

    fadeIn(targetPage);

    ui->stackedWidget->setCurrentWidget(targetPage);
}

void AuthWindow::goToLoginPage() {
    QWidget* targetPage = ui->stackedWidget->widget(0);

    fadeIn(targetPage);

    ui->stackedWidget->setCurrentWidget(targetPage);
}

void AuthWindow::onLoginResponse(bool success, const QString &message) {
    if (success) {
        qDebug() << "Login successful:" << message;
        // 处理登录成功逻辑
    } else {
        qDebug() << "Login failed:" << message;
        // 处理登录失败逻辑
    }
}

void AuthWindow::onRegisterResponse(bool success, const QString &message) {
    if (success) {
        qDebug() << "Register successful:" << message;
        // 处理注册成功逻辑
    } else {
        qDebug() << "Register failed:" << message;
        // 处理注册失败逻辑
    }
}

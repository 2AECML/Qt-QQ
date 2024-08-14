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

AuthWindow::AuthWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthWindow())
    , vcodeTimer(new QTimer(this))
    , countdownNum(60){

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

void AuthWindow::installEventFilters() {
    // 遍历 UI 中的所有按钮并安装事件过滤器
    QList<QAbstractButton*> buttons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton* button : buttons) {
        button->installEventFilter(this);
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

    connect(vcodeTimer, &QTimer::timeout, this, &AuthWindow::updateVcodeBtnText);

    // 遍历所有QLineEdit对象
    QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
    for (QLineEdit *lineEdit : lineEdits) {
        connect(lineEdit, &QLineEdit::editingFinished, this, &AuthWindow::onEditingFinished);
    }
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

    if (countdownNum > 0) {
        vcodeBtn->setText(QString("重新发送(%1)").arg(countdownNum--));
    } else {
        vcodeBtn->setText("发送验证码");
        vcodeBtn->setProperty("waiting", false);
        vcodeBtn->setEnabled(true);
        vcodeTimer->stop();
        countdownNum = 60;
    }
    // 强制重新应用样式
    vcodeBtn->style()->unpolish(vcodeBtn);
    vcodeBtn->style()->polish(vcodeBtn);
    vcodeBtn->update();
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

    // 强制重新应用样式
    loginBtn->style()->unpolish(loginBtn);
    loginBtn->style()->polish(loginBtn);
    loginBtn->update();
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
        vcodeTimer->start(1000);
        updateVcodeBtnText();
    }

    // 强制重新应用样式
    vcodeBtn->style()->unpolish(vcodeBtn);
    vcodeBtn->style()->polish(vcodeBtn);
    vcodeBtn->update();
}

void AuthWindow::onLoginBtnClicked() {
    if (mAgreeChecked == true) {
        const QString& account = ui->loginAccountInput->text();
        const QString& password = ui->loginPasswordInput->text();

        qDebug() << "账号：" << account;
        qDebug() << "密码：" << password;

        InputValidator::isAccountValid(account) ? qDebug() << "账号合法" : qDebug() << "账号不合法";
        InputValidator::isPasswordValid(password) ? qDebug() << "密码合法" : qDebug() << "密码不合法";
    }
}

void AuthWindow::onRegisterBtnClicked() {
    const QString& nickname = ui->registerNicknameInput->text();
    const QString& password = ui->registerPasswordInput->text();
    const QString& phone = ui->registerPhoneInput->text();
    const QString& vcode = ui->registerVcodeInput->text();

    qDebug() << "昵称：" << nickname;
    qDebug() << "密码：" << password;
    qDebug() << "手机号码：" << phone;
    qDebug() << "验证码：" << vcode;

    InputValidator::isNicknameValid(nickname) ? qDebug() << "昵称合法" : qDebug() << "昵称不合法";
    InputValidator::isPasswordValid(password) ? qDebug() << "密码合法" : qDebug() << "密码不合法";
    InputValidator::isPhoneValid(phone) ? qDebug() << "手机号码合法" : qDebug() << "手机号码不合法";
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

        // 强制重新应用样式
        hint->style()->unpolish(hint);
        hint->style()->polish(hint);
        hint->update();
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
                hint->setText("8-16个字符，不允许出现空格");
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }

        // 强制重新应用样式
        hint->style()->unpolish(hint);
        hint->style()->polish(hint);
        hint->update();
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

        // 强制重新应用样式
        hint->style()->unpolish(hint);
        hint->style()->polish(hint);
        hint->update();
    }
    else {
        return;
    }
    lineEdit->style()->unpolish(lineEdit);
    lineEdit->style()->polish(lineEdit);
    lineEdit->update();
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

#include "registerwidget.h"
#include "ui_registerwidget.h"
#include "inputvalidator.h"
#include <QStyle>

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWidget)
    , mVcodeTimer(new QTimer(this))
    , mCountdownNum(60) {
    ui->setupUi(this);

    setupConnection();
}

RegisterWidget::~RegisterWidget() {
    delete ui;
}

void RegisterWidget::setupConnection() {
    connect(ui->registerBtn, &QPushButton::clicked, this, &RegisterWidget::onRegisterBtnClicked);

    connect(ui->registerVcodeBtn, &QPushButton::clicked, this, &RegisterWidget::onVcodeBtnClicked);

    connect(mVcodeTimer, &QTimer::timeout, this, &RegisterWidget::updateVcodeBtnText);

    // 遍历所有QLineEdit对象
    QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
    for (QLineEdit *lineEdit : lineEdits) {
        connect(lineEdit, &QLineEdit::editingFinished, this, &RegisterWidget::onEditingFinished);
    }

    connect(ui->toLoginBtn, &QPushButton::clicked, this, [this]() {
        emit goToLoginSignal();
    });
}

void RegisterWidget::updateVcodeBtnText() {
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

void RegisterWidget::updateStyle(QWidget *w) {
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

void RegisterWidget::onVcodeBtnClicked() {
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

void RegisterWidget::onRegisterBtnClicked() {
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
        emit registerSignal(nickname, password, phone, vcode);
    }
}

void RegisterWidget::onEditingFinished() {
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

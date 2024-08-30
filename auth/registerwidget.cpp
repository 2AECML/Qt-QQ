#include "registerwidget.h"
#include "ui_registerwidget.h"
#include "inputvalidator.h"
#include <QStyle>

// 构造函数，初始化 UI 和计时器
RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent) // 调用 QWidget 的构造函数
    , ui(new Ui::RegisterWidget) // 初始化 UI 对象
    , mVcodeTimer(new QTimer(this)) // 初始化验证码计时器
    , mCountdownNum(60) { // 设置倒计时初始值为60秒
    ui->setupUi(this); // 设置 UI

    setupConnection(); // 设置信号和槽的连接
}

// 析构函数，清理资源
RegisterWidget::~RegisterWidget() {
    delete ui; // 删除 UI 对象
}

// 设置信号和槽的连接
void RegisterWidget::setupConnection() {
    // 连接注册按钮的点击信号到槽函数
    connect(ui->registerBtn, &QPushButton::clicked, this, &RegisterWidget::onRegisterBtnClicked);

    // 连接获取验证码按钮的点击信号到槽函数
    connect(ui->registerVcodeBtn, &QPushButton::clicked, this, &RegisterWidget::onVcodeBtnClicked);

    // 连接计时器的超时信号到槽函数
    connect(mVcodeTimer, &QTimer::timeout, this, &RegisterWidget::updateVcodeBtnText);

    // 遍历所有 QLineEdit 对象，连接编辑完成信号到槽函数
    QList<QLineEdit*> lineEdits = this->findChildren<QLineEdit*>();
    for (QLineEdit *lineEdit : lineEdits) {
        connect(lineEdit, &QLineEdit::editingFinished, this, &RegisterWidget::onEditingFinished);
    }

    // 连接跳转到登录页面按钮的点击信号到槽函数
    connect(ui->toLoginBtn, &QPushButton::clicked, this, [this]() {
        emit goToLoginSignal(); // 发出跳转到登录页面的信号
    });
}

// 更新验证码按钮的文本及状态
void RegisterWidget::updateVcodeBtnText() {
    QPushButton* vcodeBtn = ui->registerVcodeBtn;

    if (mCountdownNum > 0) {
        // 更新按钮文本并递减倒计时
        vcodeBtn->setText(QString("重新发送(%1)").arg(mCountdownNum--));
    } else {
        // 恢复按钮文本并重新启用按钮
        vcodeBtn->setText("发送验证码");
        vcodeBtn->setProperty("waiting", false);
        vcodeBtn->setEnabled(true);
        mVcodeTimer->stop(); // 停止计时器
        mCountdownNum = 60; // 重置倒计时
    }
    // 强制重新应用样式
    updateStyle(vcodeBtn);
}

// 更新控件的样式
void RegisterWidget::updateStyle(QWidget *w) {
    w->style()->unpolish(w); // 取消控件的当前样式
    w->style()->polish(w); // 重新应用控件的样式
    w->update(); // 更新控件显示
}

// 处理获取验证码按钮的点击事件
void RegisterWidget::onVcodeBtnClicked() {
    bool validPhone = ui->registerPhoneInput->property("valid").toBool(); // 检查手机号码是否有效

    if (!validPhone) {
        return; // 如果手机号码无效，返回
    }

    QPushButton* vcodeBtn = ui->registerVcodeBtn;

    if (!vcodeBtn->property("waiting").toBool()) {
        // 设置按钮为等待状态并禁用
        vcodeBtn->setProperty("waiting", true);
        vcodeBtn->setEnabled(false);
        mVcodeTimer->start(1000); // 启动计时器，间隔1秒
        updateVcodeBtnText(); // 更新按钮文本
    }

    updateStyle(vcodeBtn); // 更新按钮的样式
}

// 处理注册按钮的点击事件
void RegisterWidget::onRegisterBtnClicked() {
    const QString& nickname = ui->registerNicknameInput->text(); // 获取输入的昵称
    const QString& password = ui->registerPasswordInput->text(); // 获取输入的密码
    const QString& phone = ui->registerPhoneInput->text(); // 获取输入的手机号码
    const QString& vcode = ui->registerVcodeInput->text(); // 获取输入的验证码

    // qDebug() << "昵称：" << nickname; // 调试输出昵称
    // qDebug() << "密码：" << password; // 调试输出密码
    // qDebug() << "手机号码：" << phone; // 调试输出手机号码
    // qDebug() << "验证码：" << vcode; // 调试输出验证码

    bool nicknameValid = InputValidator::isNicknameValid(nickname); // 验证昵称
    bool passwordValid = InputValidator::isPasswordValid(password); // 验证密码
    bool phoneValid = InputValidator::isPhoneValid(phone); // 验证手机号码

    // nicknameValid ? qDebug() << "昵称合法" : qDebug() << "昵称不合法";
    // passwordValid ? qDebug() << "密码合法" : qDebug() << "密码不合法";
    // phoneValid ? qDebug() << "手机号码合法" : qDebug() << "手机号码不合法";

    if (nicknameValid && passwordValid && phoneValid) {
        emit registerSignal(nickname, password, phone, vcode); // 发出注册信号
    }
}

// 处理编辑完成事件
void RegisterWidget::onEditingFinished() {
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender()); // 获取发送信号的 QLineEdit 对象

    if (!lineEdit) {
        return; // 如果不是 QLineEdit 对象，返回
    }

    QString lineName = lineEdit->objectName(); // 获取控件名称

    if (lineName == "registerNicknameInput") { // 处理昵称输入框
        QLabel* hint = ui->nicknameHint; // 获取提示标签

        if (lineEdit->text().isEmpty()) {
            hint->setText("昵称不能为空"); // 昵称为空的提示
            hint->setProperty("valid", false);
            lineEdit->setProperty("valid", false);
        }
        else {
            bool valid = InputValidator::isNicknameValid(lineEdit->text()); // 验证昵称合法性

            if (valid) {
                hint->setText("昵称合法"); // 昵称合法的提示
                hint->setProperty("valid", true);
                lineEdit->setProperty("valid", true);
            }
            else {
                hint->setText("24位内任意字符，不允许全为空格"); // 昵称不合法的提示
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }

        updateStyle(hint); // 更新提示标签的样式
    }
    else if (lineName == "registerPasswordInput") { // 处理密码输入框
        QLabel* hint = ui->passwordHint; // 获取提示标签

        if (lineEdit->text().isEmpty()) {
            hint->setText("密码不能为空"); // 密码为空的提示
            hint->setProperty("valid", false);
            lineEdit->setProperty("valid", false);
        }
        else {
            bool valid = InputValidator::isPasswordValid(lineEdit->text()); // 验证密码合法性

            if (valid) {
                hint->setText("密码合法"); // 密码合法的提示
                hint->setProperty("valid", true);
                lineEdit->setProperty("valid", true);
            }
            else {
                hint->setText("8-16个字符，不允许出现空格或全为数字"); // 密码不合法的提示
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }

        updateStyle(hint); // 更新提示标签的样式
    }
    else if (lineName == "registerPhoneInput") { // 处理手机号码输入框
        QLabel* hint = ui->phoneHint; // 获取提示标签

        if (lineEdit->text().isEmpty()) {
            hint->setText("手机号码不能为空"); // 手机号码为空的提示
            hint->setProperty("valid", false);
            lineEdit->setProperty("valid", false);
        }
        else {
            bool valid = InputValidator::isPhoneValid(lineEdit->text()); // 验证手机号码合法性

            if (valid) {
                hint->setText("手机号码合法"); // 手机号码合法的提示
                hint->setProperty("valid", true);
                lineEdit->setProperty("valid", true);
            }
            else {
                hint->setText("11位手机号码"); // 手机号码不合法的提示
                hint->setProperty("valid", false);
                lineEdit->setProperty("valid", false);
            }
        }
        updateStyle(hint); // 更新提示标签的样式
    }
    else {
        return; // 如果不是有效的 QLineEdit 对象，返回
    }
    updateStyle(lineEdit); // 更新 QLineEdit 控件的样式
}

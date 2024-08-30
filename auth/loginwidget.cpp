#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QStyle>

// 构造函数，初始化 UI
LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent) // 调用 QWidget 的构造函数
    , ui(new Ui::LoginWidget) { // 初始化 UI 对象
    ui->setupUi(this); // 设置 UI

    setupConnection(); // 设置信号和槽的连接
}

// 析构函数，清理资源
LoginWidget::~LoginWidget() {
    delete ui; // 删除 UI 对象
}

// 设置信号和槽的连接
void LoginWidget::setupConnection() {
    // 连接同意按钮的点击信号到槽函数
    connect(ui->loginAgreeBtn, &QRadioButton::clicked, this, &LoginWidget::onAgreeBtnClicked);

    // 连接登录按钮的点击信号到槽函数
    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginBtnClicked);

    // 连接跳转到注册页面按钮的点击信号到槽函数
    connect(ui->toRegisterBtn, &QPushButton::clicked, this, [this]() {
        emit goToRegisterSignal(); // 发出跳转到注册页面的信号
    });
}

// 更新控件的样式
void LoginWidget::updateStyle(QWidget *w) {
    w->style()->unpolish(w); // 取消控件的当前样式
    w->style()->polish(w); // 重新应用控件的样式
    w->update(); // 更新控件显示
}

// 处理同意按钮的点击事件
void LoginWidget::onAgreeBtnClicked(bool checked) {
    QPushButton* loginBtn = ui->loginBtn; // 获取登录按钮

    mAgreeChecked = checked; // 更新同意状态

    if (checked) { // 如果同意已被选中
        loginBtn->setProperty("agreement", true); // 设置属性标记为已同意
        loginBtn->setCursor(Qt::PointingHandCursor); // 设置鼠标光标为手型
    }
    else { // 如果同意未被选中
        loginBtn->setProperty("agreement", false); // 设置属性标记为未同意
        loginBtn->setCursor(Qt::ForbiddenCursor); // 设置鼠标光标为禁止
    }

    updateStyle(loginBtn); // 更新登录按钮的样式
}

// 处理登录按钮的点击事件
void LoginWidget::onLoginBtnClicked() {
    if (mAgreeChecked == true) { // 检查是否已同意
        const QString& account = ui->loginAccountInput->text(); // 获取输入的账户名
        const QString& password = ui->loginPasswordInput->text(); // 获取输入的密码

        // qDebug() << "账号：" << account; // 调试输出账户名
        // qDebug() << "密码：" << password; // 调试输出密码

        emit loginSignal(account, password); // 发出登录信号
    }
}

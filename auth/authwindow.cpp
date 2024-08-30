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

// AuthWindow 构造函数
AuthWindow::AuthWindow(QWidget *parent)
    : BasicWidget(parent)
    , ui(new Ui::AuthWindow()) // 初始化 UI 对象
    , mLoginPage(new LoginWidget(ui->stackedWidget)) // 初始化登录页
    , mRegisterPage(new RegisterWidget(ui->stackedWidget)) // 初始化注册页
    , mWaitPage(new WaitWidget(ui->stackedWidget)) // 初始化等待页
    , mNetworkManager(new AuthNetworkManager(this)) { // 初始化网络管理器

    ui->setupUi(this); // 设置 UI

    initPages(); // 初始化页面
    setupConnection(); // 设置信号和槽的连接
    loadStyle(); // 加载样式表
}

// AuthWindow 析构函数
AuthWindow::~AuthWindow() {
    delete ui; // 删除 UI 对象
    qDebug() << "deleted AuthWindow"; // 打印调试信息
}

// 处理窗口关闭事件
void AuthWindow::closeEvent(QCloseEvent *event) {
    // 关闭网络连接
    if (mNetworkManager) {
        mNetworkManager->disconnectFromServer();
    }

    emit closed(); // 发出关闭信号

    event->accept(); // 接受事件，执行默认关闭操作
}

// 加载 QSS 文件
void AuthWindow::loadStyle() {
    QFile file(":/auth.qss"); // 打开 QSS 文件
    if (file.open(QFile::ReadOnly)) { // 如果成功打开
        QString styleSheet = file.readAll(); // 读取样式表内容
        this->setStyleSheet(styleSheet); // 设置样式表
    }
}

// 设置信号和槽连接
void AuthWindow::setupConnection() {
    // 连接关闭按钮的点击信号到槽函数
    connect(ui->closeBtn, &QToolButton::clicked, this, &AuthWindow::onCloseBtnClicked);

    // 连接最小化按钮的点击信号到槽函数
    connect(ui->minimizeBtn, &QToolButton::clicked, this, &AuthWindow::onMinimizeBtnClicked);

    // 连接登录页的登录信号到槽函数
    connect(mLoginPage, &LoginWidget::loginSignal, this, [this](const QString& account, const QString& password) {
        mNetworkManager->sendLoginRequest(account, password); // 发送登录请求
        goToWaitPage(WaitWidget::Type::Logining); // 切换到等待页
    });

    // 连接注册页的注册信号到槽函数
    connect(mRegisterPage, &RegisterWidget::registerSignal, this, [this](const QString& nickname, const QString& password, const QString& phone, const QString& vcode) {
        mNetworkManager->sendRegisterRequest(nickname, password, phone, vcode); // 发送注册请求
        goToWaitPage(WaitWidget::Type::Registration); // 切换到等待页
    });

    // 连接登录页的切换到注册页信号到槽函数
    connect(mLoginPage, &LoginWidget::goToRegisterSignal, this, &AuthWindow::goToRegisterPage);

    // 连接注册页的切换到登录页信号到槽函数
    connect(mRegisterPage, &RegisterWidget::goToLoginSignal, this, &AuthWindow::goToLoginPage);

    // 连接网络管理器的登录响应信号到槽函数
    connect(mNetworkManager, &AuthNetworkManager::loginResponse, this, &AuthWindow::onLoginResponse);

    // 连接网络管理器的注册响应信号到槽函数
    connect(mNetworkManager, &AuthNetworkManager::registerResponse, this, &AuthWindow::onRegisterResponse);

    // 连接等待页的取消信号到槽函数
    connect(mWaitPage, &WaitWidget::cancelSignal, this, [this]() {
        mNetworkManager->disconnectFromServer(); // 断开网络连接
        returnToLastPage(); // 返回到上一页
    });

    // 连接当前页面改变信号到槽函数
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &AuthWindow::onCurrentChanged);
}

// 初始化页面
void AuthWindow::initPages() {
    ui->stackedWidget->addWidget(mLoginPage); // 添加登录页
    ui->stackedWidget->addWidget(mRegisterPage); // 添加注册页
    ui->stackedWidget->addWidget(mWaitPage); // 添加等待页

    ui->stackedWidget->setCurrentWidget(mLoginPage); // 设置当前显示的页面为登录页

    mLastPageIndex = ui->stackedWidget->currentIndex(); // 记录上一个页面索引
}

// 实现页面淡出效果
void AuthWindow::fadeOut(QWidget* targetPage) const {
    if (!targetPage) {
        return;
    }

    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(targetPage); // 创建透明度效果
    targetPage->setGraphicsEffect(opacityEffect); // 应用效果

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity"); // 创建动画
    animation->setDuration(500); // 设置动画持续时间
    animation->setStartValue(1.0); // 设置动画起始透明度
    animation->setEndValue(0.0); // 设置动画结束透明度

    // 动画完成后删除效果和动画对象
    connect(animation, &QPropertyAnimation::finished, this, [opacityEffect, animation]() {
        if (opacityEffect) {
            opacityEffect->deleteLater();
        }
        if (animation) {
            animation->deleteLater();
        }
    });

    animation->start(); // 启动动画
}

// 实现页面淡入效果
void AuthWindow::fadeIn(QWidget *targetPage) const {
    if (!targetPage) {
        return;
    }

    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(targetPage); // 创建透明度效果
    targetPage->setGraphicsEffect(opacityEffect); // 应用效果

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity"); // 创建动画
    animation->setDuration(500); // 设置动画持续时间
    animation->setStartValue(0.0); // 设置动画起始透明度
    animation->setEndValue(1.0); // 设置动画结束透明度

    // 动画完成后删除效果和动画对象
    QObject::connect(animation, &QPropertyAnimation::finished, this, [animation, opacityEffect]() {
        if (opacityEffect) {
            opacityEffect->deleteLater();
        }
        if (animation) {
            animation->deleteLater();
        }
    });

    animation->start(); // 启动动画
}

// 更新窗口的样式
void AuthWindow::updateStyle(QWidget *w) {
    w->style()->unpolish(w); // 取消样式
    w->style()->polish(w); // 应用样式
    w->update(); // 更新窗口
}

// 处理关闭按钮点击事件
void AuthWindow::onCloseBtnClicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity"); // 创建动画
    animation->setDuration(100); // 设置动画持续时间
    animation->setStartValue(1.0); // 设置动画起始透明度
    animation->setEndValue(0.0); // 设置动画结束透明度
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->close(); // 关闭窗口
        this->setWindowOpacity(1.0); // 恢复透明度
    });
    animation->start(QPropertyAnimation::DeleteWhenStopped); // 启动动画并在停止时删除
}

// 处理最小化按钮点击事件
void AuthWindow::onMinimizeBtnClicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity"); // 创建动画
    animation->setDuration(200); // 设置动画持续时间
    animation->setStartValue(1.0); // 设置动画起始透明度
    animation->setEndValue(0.0); // 设置动画结束透明度
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->showMinimized(); // 最小化窗口
        this->setWindowOpacity(1.0); // 恢复透明度
    });
    animation->start(QPropertyAnimation::DeleteWhenStopped); // 启动动画并在停止时删除
}

// 切换到注册页面
void AuthWindow::goToRegisterPage() {
    mLastPageIndex = ui->stackedWidget->currentIndex(); // 记录当前页面索引

    int targetIndex = ui->stackedWidget->indexOf(mRegisterPage); // 获取注册页索引
    QWidget* targetPage = ui->stackedWidget->widget(targetIndex); // 获取注册页

    ui->stackedWidget->setCurrentWidget(targetPage); // 切换到注册页
}

// 切换到登录页面
void AuthWindow::goToLoginPage() {
    mLastPageIndex = ui->stackedWidget->currentIndex(); // 记录当前页面索引

    int targetIndex = ui->stackedWidget->indexOf(mLoginPage); // 获取登录页索引
    QWidget* targetPage = ui->stackedWidget->widget(targetIndex); // 获取登录页

    ui->stackedWidget->setCurrentWidget(targetPage); // 切换到登录页
}

// 切换到等待页面
void AuthWindow::goToWaitPage(WaitWidget::Type waitType) {
    mLastPageIndex = ui->stackedWidget->currentIndex(); // 记录当前页面索引

    mWaitPage->setType(waitType); // 设置等待页类型

    int targetIndex = ui->stackedWidget->indexOf(mWaitPage); // 获取等待页索引
    QWidget* targetPage = ui->stackedWidget->widget(targetIndex); // 获取等待页

    ui->stackedWidget->setCurrentWidget(targetPage); // 切换到等待页
}

// 返回到上一个页面
void AuthWindow::returnToLastPage() {
    ui->stackedWidget->setCurrentIndex(mLastPageIndex); // 切换到上一个页面
}

// 处理登录响应
void AuthWindow::onLoginResponse(bool success, const QString &message, const QString& accountID) {
    if (success) {
        qDebug() << "Login successful:" << message; // 登录成功
        AuthHintDialog dialog(this); // 创建提示对话框
        dialog.setHint(message); // 设置提示信息
        emit loginSuccessful(accountID); // 发出登录成功信号
        this->close(); // 关闭窗口
    } else {
        qDebug() << "Login failed:" << message; // 登录失败
        AuthHintDialog dialog; // 创建提示对话框
        dialog.setHint(message); // 设置提示信息
        connect(&dialog, &QDialog::finished, this, [this, &dialog]() {
            mNetworkManager->disconnectFromServer(); // 断开网络连接
            dialog.close(); // 关闭对话框
            returnToLastPage(); // 返回到上一页
        });
        dialog.exec(); // 显示对话框
    }
}

// 处理注册响应
void AuthWindow::onRegisterResponse(bool success, const QString& message, const QString& accountID) {
    if (success) {
        qDebug() << "Register successful:" << message; // 注册成功
        AuthHintDialog dialog; // 创建提示对话框
        dialog.setHint(message, accountID); // 设置提示信息
        connect(&dialog, &QDialog::finished, this, [this, &dialog]() {
            mNetworkManager->disconnectFromServer(); // 断开网络连接
            dialog.close(); // 关闭对话框
            returnToLastPage(); // 返回到上一页
        });
        dialog.exec(); // 显示对话框
    } else {
        qDebug() << "Register failed:" << message; // 注册失败
        AuthHintDialog dialog; // 创建提示对话框
        dialog.setHint(message); // 设置提示信息
        connect(&dialog, &QDialog::finished, this, [this, &dialog]() {
            mNetworkManager->disconnectFromServer(); // 断开网络连接
            dialog.close(); // 关闭对话框
            returnToLastPage(); // 返回到上一页
        });
        dialog.exec(); // 显示对话框
    }
}

// 处理当前页面改变事件
void AuthWindow::onCurrentChanged(int index) {
    QWidget* currentPage = ui->stackedWidget->widget(index); // 获取当前页面
    if (currentPage) {
        fadeIn(currentPage); // 对当前页面执行淡入效果
    }
}

#include "homewindow.h"
#include "ui_homewindow.h"
#include "userlistitem.h"
#include <QFile>
#include <QPropertyAnimation>
#include <QScrollBar>

// 构造函数，初始化 HomeWindow
HomeWindow::HomeWindow(const QString &accountID, QWidget *parent)
    : BasicWidget(parent)
    , ui(new Ui::HomeWindow)
    , mNetworkManager(new HomeNetworkManager(this))
    , mSelfID(accountID) {

    ui->setupUi(this);  // 设置 UI 组件

    loadSelfInfo(mSelfID);  // 加载用户的个人信息

    loadList(mSelfID);  // 加载用户列表

    setupConnection();  // 设置信号与槽连接

    loadStyle();  // 加载样式表
}

// 析构函数，清理资源
HomeWindow::~HomeWindow() {
    delete ui;  // 删除 UI 组件
    qDebug() << "deleted HomeWindow";  // 输出调试信息
}

// 关闭事件处理，关闭网络连接并发射 closed 信号
void HomeWindow::closeEvent(QCloseEvent* event) {
    if (mNetworkManager) {
        mNetworkManager->disconnect();  // 断开网络连接
    }

    emit closed();  // 发射 closed 信号

    event->accept();  // 接受关闭事件
}

// 加载样式表
void HomeWindow::loadStyle() {
    QFile file(":/home.qss");  // 打开 QSS 文件
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();  // 读取样式表
        this->setStyleSheet(styleSheet);  // 设置样式表
    }
}

// 设置信号与槽连接
void HomeWindow::setupConnection() {
    connect(ui->closeBtn, &QToolButton::clicked, this, &HomeWindow::onCloseBtnClicked);  // 关闭按钮点击事件

    connect(ui->minimizeBtn, &QToolButton::clicked, this, &HomeWindow::onMinimizeBtnClicked);  // 最小化按钮点击事件

    connect(ui->searchInput, &QLineEdit::textEdited, this, &HomeWindow::onSearchInputEdited);  // 搜索框文本编辑事件

    connect(ui->userList, &QListWidget::itemDoubleClicked, this, &HomeWindow::onListItemDoubleClicked);  // 列表项双击事件
}

// 加载个人信息
void HomeWindow::loadSelfInfo(const QString &accountID) {
    mNetworkManager->sendUserInfoRequest(accountID);  // 发送请求获取用户信息

    connect(mNetworkManager, &HomeNetworkManager::selfInfoResponse, this,
            [this](const QString& id, const QString& nickname) {
                ui->nickname->setText(nickname);  // 设置昵称
                ui->userID->setText(id);  // 设置用户ID
            });
}

// 加载用户列表
void HomeWindow::loadList(const QString &accountID) {
    mNetworkManager->sendUserListRequest();  // 发送请求获取用户列表

    connect(mNetworkManager, &HomeNetworkManager::userListItemResponse, this,
            [this, accountID](const QString& id, const QString& nickname) {

                if (id == accountID) {
                    mSelfName = nickname;  // 记录自己的昵称
                    return;
                }

                QWidget* infoWidget = new UserListItem(nickname, id, ui->userList);  // 创建用户列表项

                QListWidgetItem* listItem = new QListWidgetItem(ui->userList);  // 创建列表项
                ui->userList->addItem(listItem);  // 添加到列表
                ui->userList->setItemWidget(listItem, infoWidget);  // 设置列表项的显示部件

                int viewPostWith = ui->userList->viewport()->width();  // 获取视图宽度
                int scrollbarWith = ui->userList->verticalScrollBar()->width();  // 获取滚动条宽度

                listItem->setSizeHint(QSize(viewPostWith - scrollbarWith, 80));  // 设置列表项大小
            });
}

// 关闭按钮点击事件处理
void HomeWindow::onCloseBtnClicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");  // 创建动画对象
    animation->setDuration(100);  // 设置动画持续时间
    animation->setStartValue(1.0);  // 设置动画开始透明度
    animation->setEndValue(0.0);  // 设置动画结束透明度
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->close();  // 关闭窗口
        this->setWindowOpacity(1.0);  // 恢复透明度
    });
    animation->start(QPropertyAnimation::DeleteWhenStopped);  // 启动动画
}

// 最小化按钮点击事件处理
void HomeWindow::onMinimizeBtnClicked() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");  // 创建动画对象
    animation->setDuration(200);  // 设置动画持续时间
    animation->setStartValue(1.0);  // 设置动画开始透明度
    animation->setEndValue(0.0);  // 设置动画结束透明度
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->showMinimized();  // 最小化窗口
        this->setWindowOpacity(1.0);  // 恢复透明度
    });
    animation->start(QPropertyAnimation::DeleteWhenStopped);  // 启动动画
}

// 搜索框文本编辑事件处理
void HomeWindow::onSearchInputEdited(const QString& inputText) {
    for (int i = 0; i < ui->userList->count(); ++i) {

        QListWidgetItem* listItem = ui->userList->item(i);  // 获取列表项

        QWidget* widget = ui->userList->itemWidget(listItem);  // 获取列表项部件

        UserListItem* userItem = qobject_cast<UserListItem*>(widget);  // 将部件转换为 UserListItem

        if (userItem) {
            // 使用 UserListItem 对象进行操作
            QString id = userItem->getId();  // 获取用户ID
            QString nickname = userItem->getNickname();  // 获取用户昵称

            // 进行匹配
            if (nickname.contains(inputText, Qt::CaseInsensitive) ||
                id.contains(inputText, Qt::CaseInsensitive)) {
                // 显示匹配到的项
                listItem->setHidden(false);
            }
            else {
                // 隐藏不匹配的项
                listItem->setHidden(true);
            }
        }
    }
}

// 列表项双击事件处理
void HomeWindow::onListItemDoubleClicked(QListWidgetItem* item) {
    QString otherID;
    QString otherName;

    QWidget* widget = ui->userList->itemWidget(item);  // 获取列表项部件
    UserListItem* userItem = qobject_cast<UserListItem*>(widget);  // 将部件转换为 UserListItem

    if (userItem) {
        otherID = userItem->getId();  // 获取其他用户ID
        otherName = userItem->getNickname();  // 获取其他用户昵称
        emit chatWindowRequested(mSelfID, otherID, mSelfName, otherName);  // 发射请求聊天窗口的信号
    }
}

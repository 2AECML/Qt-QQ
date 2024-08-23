#include "homewindow.h"
#include "ui_homewindow.h"
#include "userlistitem.h"
#include <QFile>
#include <QPropertyAnimation>
#include <QScrollBar>

HomeWindow::HomeWindow(const QString &accountID, QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::HomeWindow)
    , mNetworkManager(new HomeNetworkManager(this)){

    ui->setupUi(this);

    // loadSelfInfo(accountID);

    loadList();

    setupConnection();

    loadStyle();
}

HomeWindow::~HomeWindow() {
    delete ui;
    qDebug() << "deleted HomeWindow";
}

void HomeWindow::closeEvent(QCloseEvent* event) {
    // 关闭网络连接
    if (mNetworkManager) {
        mNetworkManager->disconnect();
    }

    event->accept();

    emit closed();
}

void HomeWindow::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/home.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
    }
}

void HomeWindow::setupConnection() {
    connect(ui->closeBtn, &QToolButton::clicked, this, &HomeWindow::onCloseBtnClicked);

    connect(ui->minimizeBtn, &QToolButton::clicked, this, &HomeWindow::onMinimizeBtnClicked);

}

void HomeWindow::loadSelfInfo(const QString &accountID) {
    mNetworkManager->sendUserInfoRequest(accountID);
}

void HomeWindow::loadList() {
    mNetworkManager->sendUserListRequest();

    connect(mNetworkManager, &HomeNetworkManager::userListItemResponse, this,
            [this](const QString& id, const QString& nickname) {
        QWidget* infoWidget = new UserListItem(nickname, id, ui->userList);

        QListWidgetItem* listItem = new QListWidgetItem(ui->userList);

        ui->userList->addItem(listItem);
        ui->userList->setItemWidget(listItem, infoWidget);

        int viewPostWith = ui->userList->viewport()->width();
        int scrollbarWith = ui->userList->verticalScrollBar()->width();

        listItem->setSizeHint(QSize(viewPostWith - scrollbarWith, 80));
    });
}

void HomeWindow::onCloseBtnClicked() {
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

void HomeWindow::onMinimizeBtnClicked() {
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

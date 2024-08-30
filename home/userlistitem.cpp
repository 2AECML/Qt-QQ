#include "userlistitem.h"
#include "ui_userlistitem.h"

// 构造函数，初始化 UserListItem
UserListItem::UserListItem(const QString& nickname, const QString& id, QWidget* parent)
    : QWidget(parent)  // 调用基类 QWidget 的构造函数
    , ui(new Ui::UserListItem) {  // 创建 UI 组件
    ui->setupUi(this);  // 设置 UI 组件

    // 设置用户昵称和 ID
    ui->nickname->setText(nickname);  // 设置昵称文本
    ui->id->setText(id);  // 设置 ID 文本
}

// 析构函数，清理资源
UserListItem::~UserListItem() {
    delete ui;  // 删除 UI 组件
}

// 获取用户 ID
QString UserListItem::getId() const {
    return ui->id->text();  // 返回 ID 文本
}

// 获取用户昵称
QString UserListItem::getNickname() const {
    return ui->nickname->text();  // 返回昵称文本
}

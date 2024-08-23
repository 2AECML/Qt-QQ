#include "userlistitem.h"
#include "ui_userlistitem.h"

UserListItem::UserListItem(const QString& nickname, const QString& id, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::UserListItem) {
    ui->setupUi(this);
    ui->nickname->setText(nickname);
    ui->id->setText(id);
}

UserListItem::~UserListItem() {
    delete ui;
}

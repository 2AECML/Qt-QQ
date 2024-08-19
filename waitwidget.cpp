#include "waitwidget.h"
#include "ui_waitwidget.h"

WaitWidget::WaitWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WaitWidget) {
    ui->setupUi(this);

    setupConnection();
}

WaitWidget::~WaitWidget() {
    delete ui;
}

void WaitWidget::setType(Type type) {
    QString hintText;
    type == Type::Registration ? hintText = "注册中..." : hintText = "登陆中...";
    ui->hintLabel->setText(hintText);
}

void WaitWidget::setupConnection() {
    connect(ui->cancelBtn, &QPushButton::clicked, this, [this]() {
        emit cancelSignal();
    });
}

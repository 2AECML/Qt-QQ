#include "waitwidget.h"
#include "ui_waitwidget.h"

// 构造函数，初始化 UI
WaitWidget::WaitWidget(QWidget *parent)
    : QWidget(parent) // 调用 QWidget 的构造函数
    , ui(new Ui::WaitWidget) { // 初始化 UI 对象
    ui->setupUi(this); // 设置 UI

    setupConnection(); // 设置信号和槽的连接
}

// 析构函数，清理资源
WaitWidget::~WaitWidget() {
    delete ui; // 删除 UI 对象
}

// 设置等待提示类型
void WaitWidget::setType(Type type) {
    QString hintText;
    // 根据类型设置提示文本
    type == Type::Registration ? hintText = "注册中..." : hintText = "登陆中...";
    ui->hintLabel->setText(hintText); // 更新提示标签文本
}

// 设置信号和槽的连接
void WaitWidget::setupConnection() {
    // 连接取消按钮的点击信号到槽函数
    connect(ui->cancelBtn, &QPushButton::clicked, this, [this]() {
        emit cancelSignal(); // 发出取消信号
    });
}

#include "authhintdialog.h"
#include "ui_authhintdialog.h"
#include <QFile>
#include <QClipboard>

// AuthHintDialog 构造函数
AuthHintDialog::AuthHintDialog(QWidget *parent)
    : BasicDialog(parent) // 调用基类构造函数
    , ui(new Ui::AuthHintDialog) { // 初始化 UI 对象
    ui->setupUi(this); // 设置 UI

    setupConnection(); // 设置信号和槽的连接
    loadStyle(); // 加载样式表
}

// AuthHintDialog 析构函数
AuthHintDialog::~AuthHintDialog() {
    delete ui; // 删除 UI 对象
}

// 设置提示信息和可选的复制内容
void AuthHintDialog::setHint(const QString &hint, const QString copyContent) {
    ui->hintLabel->setText(hint); // 设置提示标签的文本
    if (copyContent.isEmpty()) { // 如果没有复制内容
        ui->copyBtn->hide(); // 隐藏复制按钮
    }
    else { // 如果有复制内容
        ui->copyBtn->show(); // 显示复制按钮
        connect(ui->copyBtn, &QPushButton::clicked, this, [this, copyContent]() {
            // 获取剪贴板对象
            QClipboard *clipboard = QApplication::clipboard();

            // 将文本设置到剪贴板
            clipboard->setText(copyContent);

            // 更换图标为成功图标
            ui->copyBtn->setIcon(QIcon(":/success.svg"));
        });
    }
}

// 加载 QSS 文件来设置对话框样式
void AuthHintDialog::loadStyle() {
    QFile file(":/auth_dialog.qss"); // 打开 QSS 文件
    if (file.open(QFile::ReadOnly)) { // 如果成功打开
        QString styleSheet = file.readAll(); // 读取样式表内容
        this->setStyleSheet(styleSheet); // 设置样式表
    }
}

// 设置信号和槽连接
void AuthHintDialog::setupConnection() {
    // 连接确认按钮的点击信号到关闭槽函数
    connect(ui->confirmBtn, &QPushButton::clicked, this, [this]() {
        close(); // 关闭对话框
    });
}

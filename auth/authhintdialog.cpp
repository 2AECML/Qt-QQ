#include "authhintdialog.h"
#include "ui_authhintdialog.h"
#include <QFile>
#include <QClipboard>

AuthHintDialog::AuthHintDialog(QWidget *parent)
    : BasicDialog(parent)
    , ui(new Ui::AuthHintDialog) {
    ui->setupUi(this);

    setupConnection();

    loadStyle();
}

AuthHintDialog::~AuthHintDialog() {
    delete ui;
}

void AuthHintDialog::setHint(const QString &hint, const QString copyContent) {
    ui->hintLabel->setText(hint);
    if (copyContent.isEmpty()) {
        ui->copyBtn->hide();
    }
    else {
        ui->copyBtn->show();
        connect(ui->copyBtn, &QPushButton::clicked, this, [this, copyContent]() {
            // 获取剪贴板对象
            QClipboard *clipboard = QApplication::clipboard();

            // 将文本设置到剪贴板
            clipboard->setText(copyContent);

            // 更换图标
            ui->copyBtn->setIcon(QIcon(":/success.svg"));
        });
    }
}

void AuthHintDialog::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/auth_dialog.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
    }
}

void AuthHintDialog::setupConnection() {
    connect(ui->confirmBtn, &QPushButton::clicked, this, [this]() {
        close();
    });
}

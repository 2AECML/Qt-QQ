#include "authhintdialog.h"
#include "ui_authhintdialog.h"
#include <QFile>

AuthHintDialog::AuthHintDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthHintDialog) {
    ui->setupUi(this);

    setupConnection();

    loadStyle();
}

AuthHintDialog::~AuthHintDialog() {
    delete ui;
}

void AuthHintDialog::setHint(const QString &hint) {
    ui->hintLabel->setText(hint);
}

void AuthHintDialog::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/qss/auth_dialog.qss");
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

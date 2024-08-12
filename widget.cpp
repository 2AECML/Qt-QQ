#include "widget.h"
#include "./ui_widget.h"
#include <QMouseEvent>
#include <QPropertyAnimation>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setupConnection();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - mDragPosition);
        event->accept();
    }
}

void LoginWindow::onCloseBtnClicked() {
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

void LoginWindow::onMinimizeBtnClicked() {
    // 最小化窗口
    this->showMinimized();
}

void LoginWindow::onAgreeBtnClicked(bool checked)
{
    QPushButton* loginBtn = ui->loginBtn;
    mAgreeChecked = checked;
    if (checked) {
        loginBtn->setStyleSheet("background-color: blue");
        loginBtn->setCursor(Qt::PointingHandCursor);
    }
    else {
        loginBtn->setStyleSheet("background-color: rgb(33, 174, 250)");
        loginBtn->setCursor(Qt::ForbiddenCursor);
    }
}

void LoginWindow::onLoginBtnClicked() {
    if (mAgreeChecked == true) {
        qDebug() << "登录按钮被点击";
    }
}

void LoginWindow::setupConnection() const {
    connect(ui->closeBtn, &QToolButton::clicked, this, &LoginWindow::onCloseBtnClicked);

    connect(ui->minimizeBtn, &QToolButton::clicked, this, &LoginWindow::onMinimizeBtnClicked);

    connect(ui->agreeBtn, &QRadioButton::clicked, this, &LoginWindow::onAgreeBtnClicked);

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginBtnClicked);
}


#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include <QMouseEvent>
#include <QPropertyAnimation>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setupConnection();
    installEventFilters();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::installEventFilters() {
    // 遍历 UI 中的所有按钮并安装事件过滤器
    QList<QAbstractButton*> buttons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton* button : buttons) {
        button->installEventFilter(this);
    }
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *event) {
    // 获取按钮
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);

    if (event->type() == QEvent::MouseButtonPress) {
        if (button) {
            qDebug() << "Mouse press on button:" << button;
            mDragEnabled = false; // 点击按钮，禁用拖动
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        if (button) {
            qDebug() << "Mouse release on button" << button;
            mDragEnabled = true;
        }
    }
    // 继续事件处理链
    return QWidget::eventFilter(obj, event);
}

void LoginWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (mDragEnabled) {
            mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            qDebug() << "Mouse press. Drag position set to:" << mDragPosition;
        } else {
            qDebug() << "Dragging disabled. Mouse press ignored.";
        }
        event->accept();
    } else {
        qDebug() << "Mouse press ignored. Button:" << event->button();
        QWidget::mousePressEvent(event); // 调用基类处理其他按键事件
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && mDragEnabled) {
        QPoint newPos = event->globalPosition().toPoint() - mDragPosition;
        move(newPos);
        qDebug() << "Mouse move. New position:" << newPos;
        event->accept();
    } else {
        qDebug() << "Mouse move ignored. Dragging enabled:" << mDragEnabled;
        QWidget::mouseMoveEvent(event); // 调用基类处理其他移动事件
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mDragEnabled = true; // 鼠标释放，重新启用拖动
        qDebug() << "Mouse released. Dragging enabled.";
        event->accept();
    } else {
        qDebug() << "Mouse release ignored. Button:" << event->button();
        QWidget::mouseReleaseEvent(event); // 调用基类处理其他释放事件
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


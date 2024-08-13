#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include <QPropertyAnimation>
#include <QFile>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {

    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);

    setupConnection();

    installEventFilters();

    loadStyle();
}

LoginWindow::~LoginWindow() {
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
            mDragEnabled = false; // 点击按钮，禁用拖动
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        if (button) {
            mDragEnabled = true;
        }
    }
    // 继续事件处理链
    return QWidget::eventFilter(obj, event);
}

void LoginWindow::paintEvent(QPaintEvent *evnet) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void LoginWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (mDragEnabled) {
            mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    } else {
        QWidget::mousePressEvent(event); // 调用基类处理其他按键事件
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && mDragEnabled) {
        QPoint newPos = event->globalPosition().toPoint() - mDragPosition;
        move(newPos);
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event); // 调用基类处理其他移动事件
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mDragEnabled = true; // 鼠标释放，重新启用拖动
        event->accept();
    } else {
        QWidget::mouseReleaseEvent(event); // 调用基类处理其他释放事件
    }
}

void LoginWindow::setupConnection() const {
    connect(ui->closeBtn, &QToolButton::clicked, this, &LoginWindow::onCloseBtnClicked);

    connect(ui->minimizeBtn, &QToolButton::clicked, this, &LoginWindow::onMinimizeBtnClicked);

    connect(ui->agreeBtn, &QRadioButton::clicked, this, &LoginWindow::onAgreeBtnClicked);

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginBtnClicked);
}

void LoginWindow::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/qss/loginstyle.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
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

void LoginWindow::onAgreeBtnClicked(bool checked)
{
    QPushButton* loginBtn = ui->loginBtn;
    mAgreeChecked = checked;
    if (checked) {
        loginBtn->setProperty("agreement", "true");
        loginBtn->setCursor(Qt::PointingHandCursor);
    }
    else {
        loginBtn->setProperty("agreement", "false");
        loginBtn->setCursor(Qt::ForbiddenCursor);
    }
    // 强制重新应用样式
    loginBtn->style()->unpolish(loginBtn);
    loginBtn->style()->polish(loginBtn);
    loginBtn->update();
}

void LoginWindow::onLoginBtnClicked() {
    if (mAgreeChecked == true) {
        qDebug() << "账号：" << ui->accountInput->text();
        qDebug() << "密码：" << ui->passwordInput->text();
    }
}

#include "basicdialog.h"
#include <QAbstractButton>
#include <QStyleOption>
#include <QPainter>

// 构造函数，初始化基本对话框
BasicDialog::BasicDialog(QWidget *parent)
    : QDialog(parent) {

    // 设置对话框为无边框窗口
    setWindowFlag(Qt::FramelessWindowHint);

    // 设置窗口背景为透明
    setAttribute(Qt::WA_TranslucentBackground);
}

// 析构函数
BasicDialog::~BasicDialog() {

}

// 处理鼠标按下事件
void BasicDialog::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // 如果启用了拖动，记录拖动开始的位置
        if (mDragEnabled) {
            mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    } else {
        // 处理其他鼠标按键事件
        QWidget::mousePressEvent(event);
    }
}

// 处理鼠标移动事件
void BasicDialog::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton && mDragEnabled) {
        // 计算并设置对话框的新位置
        QPoint newPos = event->globalPosition().toPoint() - mDragPosition;
        move(newPos);
        event->accept();
    } else {
        // 处理其他鼠标移动事件
        QWidget::mouseMoveEvent(event);
    }
}

// 处理鼠标释放事件
void BasicDialog::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // 鼠标释放时重新启用拖动
        mDragEnabled = true;
        event->accept();
    } else {
        // 处理其他鼠标释放事件
        QWidget::mouseReleaseEvent(event);
    }
}

// 事件过滤器，用于处理按钮的点击事件
bool BasicDialog::eventFilter(QObject* obj, QEvent* event) {
    // 将对象转换为 QAbstractButton 对象
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);

    if (event->type() == QEvent::MouseButtonPress) {
        // 按钮被点击时禁用拖动
        if (button) {
            mDragEnabled = false;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        // 按钮释放时重新启用拖动
        if (button) {
            mDragEnabled = true;
        }
    }
    // 继续事件处理链
    return QWidget::eventFilter(obj, event);
}

// 处理绘制事件
void BasicDialog::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    // 绘制对话框背景
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

// 处理显示事件
void BasicDialog::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    // 安装事件过滤器
    installEventFilters();
}

// 安装事件过滤器到所有按钮
void BasicDialog::installEventFilters() {
    // 遍历对话框中的所有按钮并安装事件过滤器
    QList<QAbstractButton*> buttons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton* button : buttons) {
        button->installEventFilter(this);
    }
}

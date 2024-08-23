#include "customwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QAbstractButton>

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget(parent) {

    setWindowFlag(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);
}

CustomWidget::~CustomWidget() {

}

void CustomWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (mDragEnabled) {
            mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    } else {
        QWidget::mousePressEvent(event); // 调用基类处理其他按键事件
    }
}

void CustomWidget::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton && mDragEnabled) {
        QPoint newPos = event->globalPosition().toPoint() - mDragPosition;
        move(newPos);
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event); // 调用基类处理其他移动事件
    }
}

void CustomWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        mDragEnabled = true; // 鼠标释放，重新启用拖动
        event->accept();
    } else {
        QWidget::mouseReleaseEvent(event); // 调用基类处理其他释放事件
    }
}

bool CustomWidget::eventFilter(QObject* obj, QEvent* event) {
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

void CustomWidget::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CustomWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    installEventFilters();
}

void CustomWidget::installEventFilters() {
    // 遍历 UI 中的所有按钮并安装事件过滤器
    QList<QAbstractButton*> buttons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton* button : buttons) {
        button->installEventFilter(this);
    }
}

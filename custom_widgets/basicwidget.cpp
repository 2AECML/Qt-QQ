#include "basicwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QAbstractButton>

// 构造函数，初始化 BasicWidget
BasicWidget::BasicWidget(QWidget *parent)
    : QWidget(parent) {  // 调用基类构造函数

    // 设置窗口标志为无边框窗口，确保它是最上层窗口
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // 设置窗口背景透明
    setAttribute(Qt::WA_TranslucentBackground);
}

// 析构函数
BasicWidget::~BasicWidget() {
    // 默认析构函数，不做任何处理
}

// 处理鼠标按下事件
void BasicWidget::mousePressEvent(QMouseEvent* event) {
    // 仅处理左键按下事件
    if (event->button() == Qt::LeftButton) {
        // 如果窗口已最大化，不允许拖动
        if (this->isMaximized()) {
            return;
        }

        // 如果拖动启用，计算拖动偏移量
        if (mDragEnabled) {
            mDragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
        event->accept();
    } else {
        QWidget::mousePressEvent(event);  // 调用基类处理其他按键事件
    }
}

// 处理鼠标移动事件
void BasicWidget::mouseMoveEvent(QMouseEvent* event) {
    // 如果左键按下且拖动启用
    if (event->buttons() & Qt::LeftButton && mDragEnabled) {
        // 如果窗口已最大化，不允许拖动
        if (this->isMaximized()) {
            return;
        }

        // 计算新的窗口位置并移动窗口
        QPoint newPos = event->globalPosition().toPoint() - mDragPosition;
        move(newPos);
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event);  // 调用基类处理其他移动事件
    }
}

// 处理鼠标释放事件
void BasicWidget::mouseReleaseEvent(QMouseEvent* event) {
    // 仅处理左键释放事件
    if (event->button() == Qt::LeftButton) {
        // 如果窗口已最大化，不允许拖动
        if (this->isMaximized()) {
            return;
        }

        mDragEnabled = true;  // 鼠标释放时重新启用拖动
        event->accept();
    } else {
        QWidget::mouseReleaseEvent(event);  // 调用基类处理其他释放事件
    }
}

// 事件过滤器处理
bool BasicWidget::eventFilter(QObject* obj, QEvent* event) {
    // 获取按钮对象
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);

    if (event->type() == QEvent::MouseButtonPress) {
        if (button) {
            mDragEnabled = false;  // 点击按钮时禁用拖动
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        if (button) {
            mDragEnabled = true;  // 鼠标释放后重新启用拖动
        }
    }
    // 继续事件处理链
    return QWidget::eventFilter(obj, event);
}

// 处理绘制事件
void BasicWidget::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);  // 初始化绘制选项
    QPainter painter(this);  // 创建绘图对象
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);  // 使用当前样式绘制窗口
}

// 处理显示事件
void BasicWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);  // 调用基类显示事件处理
    installEventFilters();  // 安装事件过滤器
}

// 安装事件过滤器到所有按钮
void BasicWidget::installEventFilters() {
    // 遍历 UI 中的所有按钮并安装事件过滤器
    QList<QAbstractButton*> buttons = this->findChildren<QAbstractButton*>();
    for (QAbstractButton* button : buttons) {
        button->installEventFilter(this);  // 为每个按钮安装事件过滤器
    }
}

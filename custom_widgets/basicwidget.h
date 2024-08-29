#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QObject>
#include <QEvent>
#include <QPoint>
#include <QShowEvent>

class BasicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicWidget(QWidget *parent = nullptr);
    ~BasicWidget();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void installEventFilters();

private:
    QPoint mDragPosition;
    bool mDragEnabled = true;
};

#endif // BASICWIDGET_H

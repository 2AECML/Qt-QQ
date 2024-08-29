#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QObject>
#include <QEvent>
#include <QPaintEvent>
#include <QShowEvent>
#include <QString>

class BasicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BasicDialog(QWidget *parent = nullptr);
    ~BasicDialog();

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

#endif // BASICDIALOG_H

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void installEventFilters();
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent* evnet) override;

private:
    void setupConnection() const;
    void loadStyle();

private slots:
    void onCloseBtnClicked();
    void onMinimizeBtnClicked();
    void onAgreeBtnClicked(bool checked);
    void onLoginBtnClicked();

private:
    Ui::Widget *ui;
    QPoint mDragPosition;
    bool mDragEnabled = true;
    bool mAgreeChecked = false;
};
#endif // LOGINWINDOW_H

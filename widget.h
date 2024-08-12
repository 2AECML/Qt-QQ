#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onCloseBtnClicked();
    void onMinimizeBtnClicked();
    void onAgreeBtnClicked(bool checked);
    void onLoginBtnClicked();

private:
    void setupConnection() const;

private:
    Ui::Widget *ui;
    QPoint mDragPosition;
    bool mAgreeChecked;
};
#endif // WIDGET_H

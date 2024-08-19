#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

private:
    void setupConnection();

    void updateVcodeBtnText();

    void updateStyle(QWidget* w);

private slots:
    void onVcodeBtnClicked();

    void onRegisterBtnClicked();

    void onEditingFinished();

private:
    Ui::RegisterWidget *ui;

    QTimer* mVcodeTimer;

    short int mCountdownNum;

signals:
    void registerSignal(const QString& nickname, const QString& password, const QString& phone, const QString& vcode);
    void goToLoginSignal();
};

#endif // REGISTERWIDGET_H

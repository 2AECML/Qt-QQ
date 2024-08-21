#ifndef AUTHHINTDIALOG_H
#define AUTHHINTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class AuthHintDialog;
}

class AuthHintDialog : public QDialog {
    Q_OBJECT

public:

    explicit AuthHintDialog(QWidget *parent = nullptr);
    ~AuthHintDialog();

    void setHint(const QString& hint);

protected:
    void loadStyle();

private:
    void setupConnection();

private:
    Ui::AuthHintDialog *ui;

};

#endif // AUTHHINTDIALOG_H

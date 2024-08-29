#ifndef AUTHHINTDIALOG_H
#define AUTHHINTDIALOG_H

#include "../custom_widgets/basicdialog.h"
#include <QDialog>
#include <QString>

namespace Ui {
class AuthHintDialog;
}

class AuthHintDialog : public BasicDialog {
    Q_OBJECT

public:
    explicit AuthHintDialog(QWidget *parent = nullptr);
    ~AuthHintDialog();

    void setHint(const QString &hint, const QString copyContent = QString());

protected:
    void loadStyle();

private:
    void setupConnection();

private:
    Ui::AuthHintDialog *ui;
};

#endif // AUTHHINTDIALOG_H

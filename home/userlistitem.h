#ifndef USERLISTITEM_H
#define USERLISTITEM_H

#include <QWidget>

namespace Ui {
class UserListItem;
}

class UserListItem : public QWidget {
    Q_OBJECT

public:
    UserListItem(const QString &nickname, const QString &id, QWidget* parent = nullptr);
    ~UserListItem();

private:
    Ui::UserListItem *ui;
};

#endif // USERLISTITEM_H

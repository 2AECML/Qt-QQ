#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QWidget>

namespace Ui {
class WaitWidget;
}

class WaitWidget : public QWidget {
    Q_OBJECT

public:
    enum class Type {Registration, Logining};

    explicit WaitWidget(QWidget *parent = nullptr);
    ~WaitWidget();    

    void setType(const Type type);

private:
    void setupConnection();

private:
    Ui::WaitWidget *ui;

signals:
    void cancelSignal();
};

#endif // WAITWIDGET_H

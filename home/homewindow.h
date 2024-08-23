#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "../custom_widgets/customwidget.h"
#include "homenetworkmanager.h"
#include <QCloseEvent>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public CustomWidget {
    Q_OBJECT

public:
    explicit HomeWindow(const QString& accountID, QWidget* parent = nullptr);
    ~HomeWindow();

protected:
    void closeEvent(QCloseEvent* event) override;
    void loadStyle();

private:
    void setupConnection();
    void loadSelfInfo(const QString& accountID);
    void loadList();

private slots:
    void onCloseBtnClicked();
    void onMinimizeBtnClicked();
    void onSearchInputEdited(const QString& inputText);

private:
    Ui::HomeWindow *ui;
    HomeNetworkManager* mNetworkManager;

signals:
    void closed();
};

#endif // HOMEWINDOW_H

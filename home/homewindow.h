#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "../custom_widgets/basicwidget.h"
#include "homenetworkmanager.h"
#include <QCloseEvent>
#include <QListWidget>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public BasicWidget {
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
    void loadList(const QString& accountID);

private slots:
    void onCloseBtnClicked();
    void onMinimizeBtnClicked();
    void onSearchInputEdited(const QString& inputText);
    void onListItemDoubleClicked(QListWidgetItem* item);

private:
    Ui::HomeWindow *ui;
    HomeNetworkManager* mNetworkManager;
    QString mSelfID;
    QString mSelfName;

signals:
    void closed();
    void chatWindowRequested(const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName);
};

#endif // HOMEWINDOW_H

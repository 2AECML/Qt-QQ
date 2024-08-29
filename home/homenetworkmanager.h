#ifndef HOMENETWORKMANAGER_H
#define HOMENETWORKMANAGER_H

#include "../custom_widgets/basictcpnetworkmanager.h"

class HomeNetworkManager : public BasicTcpNetworkManager
{
    Q_OBJECT
public:
    explicit HomeNetworkManager(QObject *parent = nullptr);
    void sendUserListRequest();
    void sendUserInfoRequest(const QString& accountID);

private slots:
    void onReadyRead() override;

signals:
    void userListResponse(const QJsonArray& list);
    void userListItemResponse(const QString& id, const QString& nickname);
    void selfInfoResponse(const QString& id, const QString& nickname);
};

#endif // HOMENETWORKMANAGER_H

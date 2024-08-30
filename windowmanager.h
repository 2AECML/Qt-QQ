#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

class WindowManager : public QObject {
    Q_OBJECT
public:
    explicit WindowManager(QObject* parent = nullptr);

private slots:
    // 处理成功登录
    void handleLoginSuccessful(const QString& accountID);

    // 处理窗口请求
    void handleChatWindowRequested(const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName);
};

#endif // WINDOWMANAGER_H

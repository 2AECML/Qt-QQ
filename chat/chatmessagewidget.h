#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QLabel>
#include <QMovie>

class ChatMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMessageWidget(QWidget *parent = nullptr);

    enum UserType {
        Self,    // 自己
        Other,   // 用户
        System,  // 系统
        Time,    // 时间
    };
    void setTextSuccess();
    void setText(const QString& text, const QString& time, const QSize& allSize, UserType userType);

    QSize getRealString(QString& src);
    QSize fontRect(const QString& str);

    inline QString text() const { return mMsg; }
    inline QString time() const { return mTime; }
    inline UserType userType() const { return mUserType; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString mMsg;
    QString mTime;
    QString mCurTime;

    QSize mAllSize;
    UserType mUserType = System;

    int mKuangWidth;
    int mTextWidth;
    int mSpaceWid;
    int mLineHeight;

    QRect mIconLeftRect;
    QRect mIconRightRect;
    QRect mSanjiaoLeftRect;
    QRect mSanjiaoRightRect;
    QRect mKuangLeftRect;
    QRect mKuangRightRect;
    QRect mTextLeftRect;
    QRect mTextRightRect;
    QPixmap mLeftPixmap;
    QPixmap mRightPixmap;
    QLabel* mLoading = nullptr;
    QMovie* mLoadingMovie = nullptr;
    bool mIsSending = false;
};

#endif // CHATMESSAGEWIDGET_H

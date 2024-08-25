#include "ChatMessageWidget.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QLabel>
#include <QDebug>

ChatMessageWidget::ChatMessageWidget(QWidget *parent) : QWidget(parent)
{
    QFont textFont = this->font();
    textFont.setFamily("MicrosoftYaHei");
    textFont.setPointSize(12);
    this->setFont(textFont);
    mLeftPixmap = QPixmap(":/img/Customer Copy.png");
    mRightPixmap = QPixmap(":/img/CustomerService.png");

    mLoadingMovie = new QMovie(this);
    mLoadingMovie->setFileName(":/img/loading4.gif");
    mLoading = new QLabel(this);
    mLoading->setMovie(mLoadingMovie);
    mLoading->resize(16, 16);
    mLoading->setAttribute(Qt::WA_TranslucentBackground, true);
    mLoading->setAutoFillBackground(false);
}

void ChatMessageWidget::setTextSuccess()
{
    mLoading->hide();
    mLoadingMovie->stop();
    mIsSending = true;
}

void ChatMessageWidget::setText(const QString& text, const QString& time, const QSize& allSize, ChatMessageWidget::UserType userType)
{
    mMsg = text;
    mUserType = userType;
    mTime = time;

    // 将时间从字符串转换为整型，表示从1970年1月1日到当前时间的秒数
    qint64 timeInSecs = time.toLongLong();
    // 使用 fromSecsSinceEpoch 来获取 QDateTime 对象
    mCurTime = QDateTime::fromSecsSinceEpoch(timeInSecs).toString("hh:mm");

    mAllSize = allSize;
    if (userType == UserType::Self) {
        if (!mIsSending) {
            mLoading->move(mKuangRightRect.x() - mLoading->width() - 10, mKuangRightRect.y() + mKuangRightRect.height() / 2 - mLoading->height() / 2);
            mLoading->show();
            mLoadingMovie->start();
        }
    } else {
        mLoading->hide();
    }

    this->update();
}

QSize ChatMessageWidget::fontRect(const QString& str)
{
    mMsg = str;
    int minHeight = 30;
    int iconWH = 40;
    int iconSpaceW = 20;
    int iconRectW = 5;
    int iconTMPH = 10;
    int sanJiaoW = 6;
    int kuangTMP = 20;
    int textSpaceRect = 12;
    mKuangWidth = this->width() - kuangTMP - 2 * (iconWH + iconSpaceW + iconRectW);
    mTextWidth = mKuangWidth - 2 * textSpaceRect;
    mSpaceWid = this->width() - mTextWidth;
    mIconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    mIconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);

    QSize size = getRealString(mMsg); // 整个的size

    qDebug() << "fontRect Size:" << size;
    int height = size.height() < minHeight ? minHeight : size.height();

    mSanjiaoLeftRect = QRect(iconWH + iconSpaceW + iconRectW, mLineHeight / 2, sanJiaoW, height - mLineHeight);
    mSanjiaoRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - sanJiaoW, mLineHeight / 2, sanJiaoW, height - mLineHeight);

    if (size.width() < (mTextWidth + mSpaceWid)) {
        mKuangLeftRect.setRect(mSanjiaoLeftRect.x() + mSanjiaoLeftRect.width(), mLineHeight / 4 * 3, size.width() - mSpaceWid + 2 * textSpaceRect, height - mLineHeight);
        mKuangRightRect.setRect(this->width() - size.width() + mSpaceWid - 2 * textSpaceRect - iconWH - iconSpaceW - iconRectW - sanJiaoW,
                                mLineHeight / 4 * 3, size.width() - mSpaceWid + 2 * textSpaceRect, height - mLineHeight);
    } else {
        mKuangLeftRect.setRect(mSanjiaoLeftRect.x() + mSanjiaoLeftRect.width(), mLineHeight / 4 * 3, mKuangWidth, height - mLineHeight);
        mKuangRightRect.setRect(iconWH + kuangTMP + iconSpaceW + iconRectW - sanJiaoW, mLineHeight / 4 * 3, mKuangWidth, height - mLineHeight);
    }
    int additionalPadding = 12; // 添加额外的填充
    mTextLeftRect.setRect(mKuangLeftRect.x() + textSpaceRect, mKuangLeftRect.y() + iconTMPH,
                          mKuangLeftRect.width() - 2 * textSpaceRect + additionalPadding, mKuangLeftRect.height() - 2 * iconTMPH);
    mTextRightRect.setRect(mKuangRightRect.x() + textSpaceRect, mKuangRightRect.y() + iconTMPH,
                           mKuangRightRect.width() - 2 * textSpaceRect + additionalPadding, mKuangRightRect.height() - 2 * iconTMPH);

    return QSize(size.width(), height);
}

QSize ChatMessageWidget::getRealString(QString& src)
{
    QFontMetricsF fm(this->font());
    mLineHeight = fm.lineSpacing();
    int lineCount = src.count("\n") + 1; // 初始行数
    int maxWidth = 0;

    qDebug() << "Initial src:" << src;

    // 处理没有换行符的情况
    if (lineCount == 1) {
        maxWidth = fm.boundingRect(src).width();
        if (maxWidth > mTextWidth) {
            maxWidth = mTextWidth;
            int size = mTextWidth / fm.boundingRect(" ").width();
            int num = fm.boundingRect(src).width() / mTextWidth;
            QString temp;
            for (int i = 0; i < num; i++) {
                temp += src.mid(i * size, (i + 1) * size) + "\n";
            }
            src = temp.trimmed(); // 去掉末尾多余的换行符
            lineCount = src.count("\n") + 1; // 更新行数
        }
    } else {
        // 处理已有换行符的情况
        QStringList lines = src.split("\n");
        for (const QString &line : lines) {
            int lineWidth = fm.boundingRect(line).width();
            maxWidth = std::max(lineWidth, maxWidth);
            if (lineWidth > mTextWidth) {
                maxWidth = mTextWidth;
                int size = mTextWidth / fm.boundingRect(" ").width();
                int num = lineWidth / mTextWidth;
                QString temp;
                for (int i = 0; i < num; i++) {
                    temp += line.mid(i * size, (i + 1) * size) + "\n";
                }
                src.replace(line, temp.trimmed());
                lineCount += temp.count("\n"); // 更新行数
            }
        }
    }

    int totalHeight = lineCount * mLineHeight + 2 * mLineHeight; // 增加额外高度
    qDebug() << "maxWidth:" << maxWidth;
    qDebug() << "lineCount:" << lineCount;
    qDebug() << "totalHeight:" << totalHeight;
    qDebug() << "Updated src:" << src;

    return QSize(maxWidth + mSpaceWid, totalHeight);
}


void ChatMessageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // 消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    if (mUserType == UserType::Other) { // 用户
        // 头像
        painter.drawPixmap(mIconLeftRect, mLeftPixmap);

        // 框加边
        QColor borderColor(234, 234, 234);
        painter.setBrush(QBrush(borderColor));
        painter.drawRoundedRect(mKuangLeftRect.x() - 1, mKuangLeftRect.y() - 1, mKuangLeftRect.width() + 2, mKuangLeftRect.height() + 2, 4, 4);
        // 框
        QColor boxColor(255, 255, 255);
        painter.setBrush(QBrush(boxColor));
        painter.drawRoundedRect(mKuangLeftRect, 4, 4);

        // 三角
        QPointF points[3] = {
            QPointF(mSanjiaoLeftRect.x(), 30),
            QPointF(mSanjiaoLeftRect.x() + mSanjiaoLeftRect.width(), 25),
            QPointF(mSanjiaoLeftRect.x() + mSanjiaoLeftRect.width(), 35),
        };
        QPen pen;
        pen.setColor(boxColor);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        // 三角加边
        QPen triangleBorderPen;
        triangleBorderPen.setColor(borderColor);
        painter.setPen(triangleBorderPen);
        painter.drawLine(QPointF(mSanjiaoLeftRect.x() - 1, 30), QPointF(mSanjiaoLeftRect.x() + mSanjiaoLeftRect.width(), 24));
        painter.drawLine(QPointF(mSanjiaoLeftRect.x() - 1, 30), QPointF(mSanjiaoLeftRect.x() + mSanjiaoLeftRect.width(), 36));

        // 内容
        QPen textPen;
        textPen.setColor(QColor(51, 51, 51));
        painter.setPen(textPen);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(mTextLeftRect, mMsg, option);
    } else if (mUserType == UserType::Self) { // 自己
        // 头像
        painter.drawPixmap(mIconRightRect, mRightPixmap);

        // 框
        QColor boxColor(75, 164, 242);
        painter.setBrush(QBrush(boxColor));
        painter.drawRoundedRect(mKuangRightRect, 4, 4);

        // 三角
        QPointF points[3] = {
            QPointF(mSanjiaoRightRect.x() + mSanjiaoRightRect.width(), 30),
            QPointF(mSanjiaoRightRect.x(), 25),
            QPointF(mSanjiaoRightRect.x(), 35),
        };
        QPen pen;
        pen.setColor(boxColor);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        // 内容
        QPen textPen;
        textPen.setColor(Qt::white);
        painter.setPen(textPen);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(mTextRightRect, mMsg, option);
    } else if (mUserType == UserType::Time) { // 时间
        QPen textPen;
        textPen.setColor(QColor(153, 153, 153));
        painter.setPen(textPen);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont timeFont = this->font();
        timeFont.setFamily("MicrosoftYaHei");
        timeFont.setPointSize(10);
        painter.setFont(timeFont);
        painter.drawText(this->rect(), mCurTime, option);
    }
}

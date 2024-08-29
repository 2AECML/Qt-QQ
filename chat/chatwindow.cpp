#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "emoticonpanel.h"
#include "chatmessage.h"
#include <QFile>
#include <QIcon>
#include <QCursor>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QTextCursor>
#include <QDateTime>
#include <QColor>
#include <QColorDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QBuffer>
#include <QImageWriter>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

ChatWindow::ChatWindow(const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName, QWidget* parent)
    : BasicWidget(parent)
    , ui(new Ui::ChatWindow)
    , mNetworkManager(new ChatNetworkManager(selfID, this))
    , mDatabaseManager(new ChatDatabaseManager(selfID, this))
    , mSelfID(selfID)
    , mOtherID(otherID)
    , mSelfName(selfName)
    , mOtherName(otherName) {

    ui->setupUi(this);

    ui->nicknameLabel->setText(otherName);

    setupConnection();

    loadStyle();

    loadHistory();

}

ChatWindow::~ChatWindow() {
    delete ui;
    qDebug() << "deleted ChatWindow";
}

void ChatWindow::setupConnection() {
    connect(ui->minimizeBtn, &QPushButton::clicked, this, [this]() {
        this->showMinimized();
    });

    connect(ui->maxmizeBtn, &QPushButton::clicked, this, [this]() {
        if (isMaximized()) {
            showNormal(); // 恢复到正常状态
            ui->maxmizeBtn->setIcon(QIcon(":/maximization-white.svg"));
        } else {
            showMaximized(); // 最大化窗口
            ui->maxmizeBtn->setIcon(QIcon(":/maximization-cancel-white.svg"));
        }
    });

    connect(ui->closeBtn, &QPushButton::clicked, this, [this]() {
        this->close();
    });

    connect(ui->emoticonBtn, &QToolButton::clicked, this, [this]() {
        EmoticonPanel* emoticonPanel = new EmoticonPanel(this);
        connect(emoticonPanel, &EmoticonPanel::emoticonSelected, this, [this](const QString &emoticon) {
            QTextCursor cursor = ui->textEditArea->textCursor();
            cursor.insertHtml(emoticon);
        });

        // 先调用 show() 显示面板
        emoticonPanel->show();

        // 获取面板的大小和鼠标位置
        QSize panelSize = emoticonPanel->size();
        QPoint mousePos = QCursor::pos();

        // 计算左下角的位置
        QPoint panelPos(mousePos.x(), mousePos.y() - panelSize.height());

        // 移动面板到计算的位置
        emoticonPanel->move(panelPos);
    });

    connect(ui->imageBtn, &QToolButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.xpm *.jpg)");
        if (!filePath.isEmpty()) {
            sendMessage(filePath, ChatMessage::Image);
        }
    });

    connect(ui->fileBtn, &QToolButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "All Files (*)");
        if (!filePath.isEmpty()) {
            sendMessage(filePath, ChatMessage::File);
        }
    });

    connect(ui->fontBtn, &QToolButton::clicked, this, [this]() {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, ui->textEditArea->font(), this);
        if (ok) {
            ui->textEditArea->setFont(font);
            mFont = font;
        }
    });

    connect(ui->colorBtn, &QToolButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor();

        if (color.isValid()) {
            QTextCharFormat format;
            format.setForeground(color);
            QTextCursor cursor(ui->textEditArea->document());
            cursor.select(QTextCursor::Document);
            cursor.mergeCharFormat(format);
            ui->textEditArea->setTextCursor(cursor);
            mFontColor = color;
        }
    });

    connect(ui->sendBtn, &QPushButton::clicked, this, [this]() {
        if (ui->textEditArea->toPlainText().isEmpty()) {
            return;
        }

        sendMessage(ui->textEditArea->toPlainText());

        ui->textEditArea->clear();
    });

    connect(mNetworkManager, &ChatNetworkManager::messageReceived, this, [this](const ChatMessage& chatMessage) {
        appendMessageToChat(chatMessage);
        // 保存历史记录
        mDatabaseManager->saveChatHistory(chatMessage);
    });
}

void ChatWindow::loadStyle() {
    // 加载 QSS 文件
    QFile file(":/chat.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = file.readAll();
        this->setStyleSheet(styleSheet);
    }
}

void ChatWindow::closeEvent(QCloseEvent *event) {
    // 关闭网络连接
    if (mNetworkManager) {
        mNetworkManager->disconnect();
    }

    emit closed();

    event->accept();
}

void ChatWindow::loadHistory() {
    QList<ChatMessage> chatList = mDatabaseManager->getChatHistory(mOtherID);

    for (auto& chatMessage : chatList) {
        appendMessageToChat(chatMessage, true);
        qDebug() << "Load history: " << chatMessage.content;
    }
}

void ChatWindow::sendMessage(const QString& message, const ChatMessage::Type type) {

    ChatMessage chatMessage;

    chatMessage.type = type;
    chatMessage.senderID = mSelfID;
    chatMessage.receiverID = mOtherID;
    chatMessage.content = message;
    chatMessage.timestamp = QDateTime::currentDateTime();
    chatMessage.font = mFont;
    chatMessage.fontColor = mFontColor;

    appendMessageToChat(chatMessage);
    mNetworkManager->sendChatMessage(chatMessage);

    // 保存历史记录
    mDatabaseManager->saveChatHistory(chatMessage);
}

void ChatWindow::appendMessageToChat(const ChatMessage& chatMessage, bool isHistory) {

    QDateTime curTime = chatMessage.timestamp;

    qint64 intervalSeconds = std::numeric_limits<qint64>::max();

    if (!mLastSendTime.isNull()) {
        intervalSeconds = mLastSendTime.secsTo(curTime);
    }
    mLastSendTime = curTime;

    QString timeInfo;

    if (intervalSeconds > 120) {
        timeInfo = QString("<div style='text-align:center; color: gray; font-size: 11px; margin: 0px'>"
                           "%1"
                           "</div>")
                       .arg(chatMessage.timestamp.toString("MM-dd hh:mm"));
    }

    QString formattedContent;

    switch (chatMessage.type) {
    case ChatMessage::Text:
        formattedContent = chatMessage.content;
        break;
    case ChatMessage::Image: {
        QString localImagePath;

        if (chatMessage.senderID == mSelfID) {
            localImagePath = chatMessage.content;
        }
        else if (isHistory) {
            // 提取文件名
            QString fileName = QFileInfo(QUrl(chatMessage.content).path()).fileName();

            // 获取完整的保存路径
            localImagePath = QDir(mLocalImagePath).filePath(fileName);
            qDebug() << "History locaoImagePath: " << localImagePath;
        }
        else {
            localImagePath = downloadImage(chatMessage.content);
        }

        if (localImagePath.isEmpty()) {
            formattedContent = "Image not found";
        }
        else {
            QImage image(localImagePath);

            const double hwRatio = static_cast<double>(image.height()) / image.width();
            const int widthLimit = 300;
            const int heightLimit = 200;

            int width = image.width();
            int height = image.height();

            if (width > widthLimit) {
                width = widthLimit;
                height = width * hwRatio;
            }
            if (height > heightLimit) {
                height = heightLimit;
                width = height / hwRatio;
            }

            formattedContent = QString("<img src='%1' alt='Image' width='%2' height='%3'>")
                                   .arg(localImagePath, QString::number(width), QString::number(height));

            qDebug() << formattedContent;
        }
        break;
    }
    case ChatMessage::File:
        QString fileName = chatMessage.receivedFileName.isEmpty() ? QFileInfo(chatMessage.content).fileName() : chatMessage.receivedFileName;
        formattedContent = "<a href='" + chatMessage.content + "'>" + fileName + "</a>";
        break;
    }

    bool isSelf = chatMessage.senderID == mSelfID;

    // 格式化消息内容
    QString formattedMessage = QString("<div style='margin: %1; text-align: %2;'>"
                                       "<b style='color: %3;'>%4:</b> "
                                       "<br>"
                                       "<span style='font-family: %5; font-size: %6px; font-weight: %7; color: %8'>"
                                       "%9"
                                       "</span>"
                                       "</div>")
                                   .arg(isSelf ? "5px 10px 12px 120px" : "5px 120px 12px 10px",
                                        isSelf ? "right" : "left",
                                        isSelf ? "#4284db" : "#29eac4",
                                        isSelf ? mSelfName : mOtherName,
                                        chatMessage.font.family(),
                                        QString::number(static_cast<int>(chatMessage.font.pointSize() * 1.8)),
                                        QString::number(chatMessage.font.weight()),
                                        chatMessage.fontColor.name(QColor::QColor::HexRgb),
                                        formattedContent
                                        );

    // 将格式化后的消息插入到聊天显示区域
    if (ui->textShowArea->toPlainText().isEmpty()) {
        ui->textShowArea->setHtml(timeInfo + formattedMessage);
    }
    else {
        ui->textShowArea->setHtml(ui->textShowArea->toHtml() + timeInfo + formattedMessage);
    }

    // qDebug() << ui->textShowArea->toHtml();

    // 更新面板
    ui->textShowArea->update();

    // 确保滚动条到达消息底部
    ui->textShowArea->moveCursor(QTextCursor::End);

}

QString ChatWindow::downloadImage(const QString &urlString) {
    // 使用 QtConcurrent::run 在后台线程中处理下载
    QFuture<QString> future = QtConcurrent::run([=]() {
        QNetworkAccessManager manager;
        QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(urlString)));

        // 等待下载完成
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QByteArray imageData = reply->readAll();
        QImage image;
        if (!image.loadFromData(imageData)) {
            qWarning() << "Failed to load image from data.";
            reply->deleteLater();
            return QString();
        }

        // 提取文件名
        QString fileName = QFileInfo(QUrl(urlString).path()).fileName();

        // 生成完整的保存路径
        QString fullPath = QDir(mLocalImagePath).filePath(fileName);

        // 确保文件夹存在
        QDir dir;
        if (!dir.exists(mLocalImagePath)) {
            if (!dir.mkpath(mLocalImagePath)) {
                qWarning() << "Failed to create directory:" << mLocalImagePath;
                reply->deleteLater();
                return QString();
            }
        }

        // 保存图像到本地
        if (!image.save(fullPath, "JPEG")) {
            qWarning() << "Failed to save image to" << fullPath;
            reply->deleteLater();
            return QString();
        }

        reply->deleteLater();
        return QFileInfo(fullPath).absoluteFilePath();
    });

    return future.result();  // 阻塞直到后台线程完成
}

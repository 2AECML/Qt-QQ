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

// 构造函数，初始化聊天窗口及其组件
ChatWindow::ChatWindow(const QString& selfID, const QString& otherID, const QString& selfName, const QString& otherName, QWidget* parent)
    : BasicWidget(parent)
    , ui(new Ui::ChatWindow) // 初始化 UI 组件
    , mNetworkManager(new ChatNetworkManager(selfID, this)) // 创建网络管理器实例
    , mDatabaseManager(new ChatDatabaseManager(selfID, this)) // 创建数据库管理器实例
    , mSelfID(selfID) // 保存自身 ID
    , mOtherID(otherID) // 保存对方 ID
    , mSelfName(selfName) // 保存自身名称
    , mOtherName(otherName) { // 保存对方名称

    ui->setupUi(this); // 设置 UI

    ui->nicknameLabel->setText(otherName); // 设置对方名称标签

    setupConnection(); // 设置信号槽连接

    loadStyle(); // 加载样式表

    loadHistory(); // 加载聊天记录
}

// 析构函数，删除 UI 组件
ChatWindow::~ChatWindow() {
    delete ui;
    qDebug() << "deleted ChatWindow"; // 输出调试信息
}

// 设置信号槽连接
void ChatWindow::setupConnection() {
    // 最小化按钮的点击事件
    connect(ui->minimizeBtn, &QPushButton::clicked, this, [this]() {
        this->showMinimized(); // 最小化窗口
    });

    // 最大化按钮的点击事件
    connect(ui->maxmizeBtn, &QPushButton::clicked, this, [this]() {
        if (isMaximized()) { // 如果窗口已经最大化
            showNormal(); // 恢复到正常状态
            ui->maxmizeBtn->setIcon(QIcon(":/maximization-white.svg")); // 设置最大化按钮的图标
        } else {
            showMaximized(); // 最大化窗口
            ui->maxmizeBtn->setIcon(QIcon(":/maximization-cancel-white.svg")); // 设置恢复按钮的图标
        }
    });

    // 关闭按钮的点击事件
    connect(ui->closeBtn, &QPushButton::clicked, this, [this]() {
        this->close(); // 关闭窗口
    });

    // 表情按钮的点击事件
    connect(ui->emoticonBtn, &QToolButton::clicked, this, [this]() {
        EmoticonPanel* emoticonPanel = new EmoticonPanel(this); // 创建表情面板
        connect(emoticonPanel, &EmoticonPanel::emoticonSelected, this, [this](const QString &emoticon) {
            QTextCursor cursor = ui->textEditArea->textCursor(); // 获取文本光标
            cursor.insertHtml(emoticon); // 插入选中的表情
        });

        // 显示表情面板
        emoticonPanel->show();

        // 获取面板的大小和鼠标位置
        QSize panelSize = emoticonPanel->size();
        QPoint mousePos = QCursor::pos();

        // 计算面板的位置
        QPoint panelPos(mousePos.x(), mousePos.y() - panelSize.height());

        // 移动面板到计算的位置
        emoticonPanel->move(panelPos);
    });

    // 图片按钮的点击事件
    connect(ui->imageBtn, &QToolButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.xpm *.jpg)"); // 打开文件对话框选择图片
        if (!filePath.isEmpty()) {
            sendMessage(filePath, ChatMessage::Image); // 发送图片消息
        }
    });

    // 文件按钮的点击事件
    connect(ui->fileBtn, &QToolButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "All Files (*)"); // 打开文件对话框选择文件
        if (!filePath.isEmpty()) {
            sendMessage(filePath, ChatMessage::File); // 发送文件消息
        }
    });

    // 字体按钮的点击事件
    connect(ui->fontBtn, &QToolButton::clicked, this, [this]() {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, ui->textEditArea->font(), this); // 打开字体选择对话框
        if (ok) {
            ui->textEditArea->setFont(font); // 设置文本编辑区字体
            mFont = font; // 保存字体
        }
    });

    // 颜色按钮的点击事件
    connect(ui->colorBtn, &QToolButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(); // 打开颜色选择对话框

        if (color.isValid()) {
            QTextCharFormat format;
            format.setForeground(color); // 设置字体颜色
            QTextCursor cursor(ui->textEditArea->document());
            cursor.select(QTextCursor::Document);
            cursor.mergeCharFormat(format); // 合并字体格式
            ui->textEditArea->setTextCursor(cursor); // 更新文本光标
            mFontColor = color; // 保存颜色
        }
    });

    // 发送按钮的点击事件
    connect(ui->sendBtn, &QPushButton::clicked, this, [this]() {
        if (ui->textEditArea->toPlainText().isEmpty()) { // 如果文本为空
            return; // 不做任何操作
        }

        sendMessage(ui->textEditArea->toPlainText()); // 发送文本消息

        ui->textEditArea->clear(); // 清空文本编辑区
    });

    // 处理接收到的消息
    connect(mNetworkManager, &ChatNetworkManager::messageReceived, this, [this](const ChatMessage& chatMessage) {
        appendMessageToChat(chatMessage); // 将消息添加到聊天窗口
        mDatabaseManager->saveChatHistory(chatMessage); // 保存聊天记录
    });
}

// 加载样式表
void ChatWindow::loadStyle() {
    QFile file(":/chat.qss"); // 打开样式表文件
    if (file.open(QFile::ReadOnly)) { // 如果文件打开成功
        QString styleSheet = file.readAll(); // 读取样式表内容
        this->setStyleSheet(styleSheet); // 设置样式表
    }
}

// 处理窗口关闭事件
void ChatWindow::closeEvent(QCloseEvent *event) {
    if (mNetworkManager) {
        mNetworkManager->disconnect(); // 关闭网络连接
    }

    emit closed(); // 发出关闭信号

    event->accept(); // 接受关闭事件
}

// 加载聊天历史记录
void ChatWindow::loadHistory() {
    QList<ChatMessage> chatList = mDatabaseManager->getChatHistory(mOtherID); // 获取聊天记录

    for (auto& chatMessage : chatList) {
        appendMessageToChat(chatMessage, true); // 将历史记录添加到聊天窗口
        qDebug() << "Load history: " << chatMessage.content; // 输出加载的聊天记录
    }
}

// 发送聊天消息
void ChatWindow::sendMessage(const QString& message, const ChatMessage::Type type) {
    ChatMessage chatMessage;

    chatMessage.type = type; // 设置消息类型
    chatMessage.senderID = mSelfID; // 设置发送者ID
    chatMessage.receiverID = mOtherID; // 设置接收者ID
    chatMessage.content = message; // 设置消息内容
    chatMessage.timestamp = QDateTime::currentDateTime(); // 设置时间戳
    chatMessage.font = mFont; // 设置字体
    chatMessage.fontColor = mFontColor; // 设置字体颜色

    appendMessageToChat(chatMessage); // 将消息添加到聊天窗口
    mNetworkManager->sendChatMessage(chatMessage); // 发送消息

    mDatabaseManager->saveChatHistory(chatMessage); // 保存聊天记录
}

// 将消息添加到聊天窗口
void ChatWindow::appendMessageToChat(const ChatMessage& chatMessage, bool isHistory) {
    QDateTime curTime = chatMessage.timestamp;

    qint64 intervalSeconds = std::numeric_limits<qint64>::max();

    if (!mLastSendTime.isNull()) {
        intervalSeconds = mLastSendTime.secsTo(curTime); // 计算上一次消息到当前消息的时间间隔
    }
    mLastSendTime = curTime;

    QString timeInfo;

    if (intervalSeconds > 120) { // 如果时间间隔大于 120 秒
        timeInfo = QString("<div style='text-align:center; color: gray; font-size: 11px; margin: 0px'>"
                           "%1"
                           "</div>")
                       .arg(chatMessage.timestamp.toString("MM-dd hh:mm")); // 格式化时间
    }

    QString formattedContent;

    switch (chatMessage.type) { // 根据消息类型格式化内容
    case ChatMessage::Text:
        formattedContent = chatMessage.content; // 文本消息
        break;
    case ChatMessage::Image: {
        QString localImagePath;

        if (chatMessage.senderID == mSelfID) {
            localImagePath = chatMessage.content; // 发送者发送的图片使用原始路径
        }
        else if (isHistory) { // 如果是历史记录
            QString fileName = QFileInfo(QUrl(chatMessage.content).path()).fileName(); // 提取文件名

            localImagePath = QDir(mLocalImagePath).filePath(fileName); // 获取本地图片路径
            qDebug() << "History locaoImagePath: " << localImagePath;
        }
        else {
            localImagePath = downloadImage(chatMessage.content); // 下载图片
        }

        if (localImagePath.isEmpty()) { // 如果图片路径为空
            formattedContent = "Image not found"; // 显示图片未找到
        }
        else {
            QImage image(localImagePath);

            const double hwRatio = static_cast<double>(image.height()) / image.width(); // 计算高度宽度比
            const int widthLimit = 300;
            const int heightLimit = 200;

            int width = image.width();
            int height = image.height();

            if (width > widthLimit) { // 如果宽度超出限制
                width = widthLimit;
                height = width * hwRatio;
            }
            if (height > heightLimit) { // 如果高度超出限制
                height = heightLimit;
                width = height / hwRatio;
            }

            formattedContent = QString("<img src='%1' alt='Image' width='%2' height='%3'>")
                                   .arg(localImagePath, QString::number(width), QString::number(height)); // 格式化图片标签

            qDebug() << formattedContent;
        }
        break;
    }
    case ChatMessage::File:
        QString fileName = chatMessage.receivedFileName.isEmpty() ? QFileInfo(chatMessage.content).fileName() : chatMessage.receivedFileName;
        formattedContent = "<a href='" + chatMessage.content + "'>" + fileName + "</a>"; // 格式化文件链接
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

    // 更新面板
    ui->textShowArea->update();

    // 确保滚动条到达消息底部
    ui->textShowArea->moveCursor(QTextCursor::End);
}

// 下载图片
QString ChatWindow::downloadImage(const QString &urlString) {
    qDebug() << "Downloading...";

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
            qWarning() << "Failed to load image from data."; // 输出警告信息
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
                qWarning() << "Failed to create directory:" << mLocalImagePath; // 输出警告信息
                reply->deleteLater();
                return QString();
            }
        }

        // 保存图像到本地
        if (!image.save(fullPath, "JPEG")) {
            qWarning() << "Failed to save image to" << fullPath; // 输出警告信息
            reply->deleteLater();
            return QString();
        }

        reply->deleteLater();
        return QFileInfo(fullPath).absoluteFilePath(); // 返回保存的图像路径
    });

    return future.result();  // 阻塞直到后台线程完成
}

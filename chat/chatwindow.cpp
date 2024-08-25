#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "emoticonpanel.h"
#include <QFile>
#include <QIcon>
#include <QCursor>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QTextCursor>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

ChatWindow::ChatWindow(const QString &selfID, const QString &otherID, QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ChatWindow)
    , mSelfID(selfID)
    , mOtherID(otherID) {

    ui->setupUi(this);

    ui->nicknameLabel->setText(mOtherID);

    setupConnection();

    loadStyle();
}

ChatWindow::~ChatWindow() {
    delete ui;
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
        QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.xpm *.jpg)");
        if (!fileName.isEmpty()) {
            QTextCursor cursor = ui->textEditArea->textCursor();
            cursor.insertHtml("<img src='" + fileName + "' width='100'/>"); // 调整图片大小
        }
    });

    connect(ui->fileBtn, &QToolButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, "选择文件", "", "All Files (*)");
        if (!fileName.isEmpty()) {
            QTextCursor cursor = ui->textEditArea->textCursor();
            cursor.insertHtml("<a href='file://" + fileName + "'>" + QFileInfo(fileName).fileName() + "</a>");
        }
    });

    connect(ui->fontBtn, &QToolButton::clicked, this, [this]() {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, ui->textEditArea->font(), this);
        if (ok) {
            ui->textEditArea->setFont(font);
        }
    });

    connect(ui->sendBtn, &QPushButton::clicked, this, [this]() {
        QString html = ui->textEditArea->toHtml();

        // 正则表达式匹配 <p> 标签及其内容
        QRegularExpression re("<p[^>]*>.*?</p>");
        QRegularExpressionMatch match = re.match(html);

        QString pTag;
        if (match.hasMatch()) {
            pTag = match.captured(0); // 提取完整的 <p> 标签及其内容
            qDebug() << "Extracted <p> tag with content:" << pTag;
        } else {
            qDebug() << "No <p> tag found.";
        }

        sendMessage(pTag);
        ui->textEditArea->clear();
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

void ChatWindow::sendMessage(const QString& htmlMessage) {
    ChatMessage message;
    message.senderID = mSelfID;
    message.receiverID = mOtherID;
    message.content = htmlMessage;
    message.timestamp = QDateTime::currentDateTime();

    appendMessageToChat(message);
}

void ChatWindow::appendMessageToChat(const ChatMessage& htmlMessage) {

    // 格式化消息内容
    QString formattedMessage;

    // 添加发送者信息和时间
    QString senderInfo = QString("<b style='color: #007bff;'>%1:</b> ")
                             .arg(htmlMessage.senderID); // 根据需要更改格式

    QString timestamp = QString("<i>[%1]</i>")
                            .arg(htmlMessage.timestamp.toString("MM-dd hh:mm")); // 时间戳格式化

    // 构造完整消息
    formattedMessage = QString(R"(<div style='background-color: #f0f0f0; padding: 10px; margin-bottom: 5px; border-radius: 5px;'>
                               <b style='color: #007bff;'>%1:</b> <i>[%2]</i><br>%3
                               </div>)")
                           .arg(senderInfo, timestamp, htmlMessage.content);

    // qDebug() << formattedMessage;

    // 将格式化后的消息插入到聊天显示区域
    ui->textShowEdit->append(formattedMessage);

    qDebug() << ui->textShowEdit->toHtml();

    // 确保滚动条到达消息底部
    ui->textShowEdit->moveCursor(QTextCursor::End);
}

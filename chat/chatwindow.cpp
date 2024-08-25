#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "emoticonpanel.h"
#include "chatmessagewidget.h"
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
        QString message = ui->textEditArea->toPlainText();
        if (message.isEmpty()) {
            return;
        }

        sendMessage(message);
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

void ChatWindow::sendMessage(const QString& message) {
    ChatMessage chatMessage;
    chatMessage.senderID = mSelfID;
    chatMessage.receiverID = mOtherID;
    chatMessage.content = message;
    chatMessage.timestamp = QDateTime::currentDateTime();

    appendMessageToChat(chatMessage);
}

void ChatWindow::appendMessageToChat(const ChatMessage& chatMessage) {
    ChatMessageWidget* messageWidget = new ChatMessageWidget(ui->textShowList);

    messageWidget->setFixedWidth(this->width());
    QSize size = messageWidget->fontRect(chatMessage.content);

    QListWidgetItem* item = new QListWidgetItem(ui->textShowList);
    item->setSizeHint(size);

    messageWidget->setText(chatMessage.content, chatMessage.timestamp.toString(), size, ChatMessageWidget::Self);

    ui->textShowList->setItemWidget(item, messageWidget);
}

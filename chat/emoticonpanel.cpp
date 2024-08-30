#include "emoticonpanel.h"
#include <QGridLayout>
#include <QPushButton>

// 构造函数，初始化表情面板
EmoticonPanel::EmoticonPanel(QWidget *parent)
    : QWidget(parent) {

    // 设置窗口属性，使其为弹出窗口，没有边框，并保持在最上层
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // 创建一个网格布局
    QGridLayout* layout = new QGridLayout(this);

    // 更新后的表情符号列表
    QList<QString> emoticons = {
        "😀", "😁", "😂", "🤣", "😅", "😆", "😉", "😊", "😋", "😎",
        "😍", "😘", "🥰", "🙂", "🤗", "🤩", "🤔", "🤨", "😐", "🙄",
        "😏", "😣", "😥", "😮", "🤐", "😪", "🥱", "😴", "😜", "🤤",
        "🙃", "🤑", "😖", "😤", "😢", "😭", "😨", "😩", "🤯", "😰",
        "😱", "🥵", "🥶", "😳", "😵", "😡", "😷", "🤮", "😇", "🥺",
        "🤠", "🤡", "🤭", "🤓", "😈", "💀", "👻", "👽", "👹", "👺"
    };

    // 创建表情按钮，并将其添加到网格布局中
    for (int i = 0; i < emoticons.size(); ++i) {
        QPushButton *button = new QPushButton(emoticons[i], this); // 创建按钮并设置表情符号
        button->setFixedSize(40, 40); // 设置按钮大小
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: #f0f0f0;"
            "    border: 1px solid #d0d0d0;"
            "    border-radius: 8px;"
            "    font-size: 20px;" // 调整字体大小
            "    padding: 0px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e0e0e0;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #d0d0d0;"
            "}"
            );
        connect(button, &QPushButton::clicked, this, [this, emoticons, i]() {
            emit emoticonSelected(emoticons[i]);
        });
        layout->addWidget(button, i / 10, i % 10); // 将按钮添加到布局中
    }
    setLayout(layout); // 设置布局
}

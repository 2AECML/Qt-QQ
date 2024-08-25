#include "emoticonpanel.h"
#include <QGridLayout>
#include <QPushButton>

EmoticonPanel::EmoticonPanel(QWidget *parent)
    : QWidget(parent) {

    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QGridLayout* layout = new QGridLayout(this);

    QList<QString> emoticons = {
        "🙂", "😀", "😔", "🤯", "😟", "😂", "🤣", "😊", "😇", "😍",
        "😎", "🤔", "😏", "😣", "😢", "😥", "🤗", "🤩", "🥺", "😬",
        "😰", "😱", "😳", "🤯", "😵", "🤤", "😪", "🥵", "🥶", "🤑",
        "🤠", "😈", "👿", "👹", "👺", "🤡", "👻", "💀", "👽", "☠️"
    };

    for (int i = 0; i < emoticons.size(); ++i) {
        QPushButton *button = new QPushButton(emoticons[i], this);
        button->setFixedSize(40, 40);
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
        layout->addWidget(button, i / 10, i % 10);
    }
    setLayout(layout);
}

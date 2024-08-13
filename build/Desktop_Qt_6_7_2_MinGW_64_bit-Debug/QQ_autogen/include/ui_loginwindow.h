/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *settingBtn;
    QToolButton *minimizeBtn;
    QToolButton *closeBtn;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *profileImg;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;
    QLineEdit *accountInput;
    QLineEdit *passwordInput;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_6;
    QRadioButton *agreeBtn;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_2;
    QPushButton *loginBtn;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *scanBtn;
    QFrame *line;
    QPushButton *moreBtn;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(300, 420);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        Widget->setMinimumSize(QSize(300, 420));
        Widget->setMaximumSize(QSize(16777215, 16777215));
        Widget->setSizeIncrement(QSize(0, 0));
        Widget->setBaseSize(QSize(0, 0));
        Widget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(Widget);
        verticalLayout_2->setSpacing(7);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(Widget);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 10);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        settingBtn = new QToolButton(widget);
        settingBtn->setObjectName("settingBtn");
        settingBtn->setMinimumSize(QSize(0, 0));
        settingBtn->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setStyleStrategy(QFont::PreferDefault);
        settingBtn->setFont(font);
        settingBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        settingBtn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(settingBtn);

        minimizeBtn = new QToolButton(widget);
        minimizeBtn->setObjectName("minimizeBtn");
        minimizeBtn->setMinimumSize(QSize(0, 0));
        minimizeBtn->setMaximumSize(QSize(16777215, 16777215));
        minimizeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        minimizeBtn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(minimizeBtn);

        closeBtn = new QToolButton(widget);
        closeBtn->setObjectName("closeBtn");
        closeBtn->setMinimumSize(QSize(0, 0));
        closeBtn->setMaximumSize(QSize(16777215, 16777215));
        closeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        closeBtn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(closeBtn);


        verticalLayout_2->addWidget(widget);

        widget_2 = new QWidget(Widget);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        widget_2->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(25, 10, 25, 25);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        profileImg = new QLabel(widget_2);
        profileImg->setObjectName("profileImg");
        profileImg->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(profileImg->sizePolicy().hasHeightForWidth());
        profileImg->setSizePolicy(sizePolicy2);
        profileImg->setMinimumSize(QSize(80, 80));
        profileImg->setMaximumSize(QSize(80, 80));
        profileImg->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        profileImg->setPixmap(QPixmap(QString::fromUtf8(":/images/QQ.svg")));
        profileImg->setScaledContents(true);
        profileImg->setWordWrap(false);
        profileImg->setMargin(10);
        profileImg->setOpenExternalLinks(false);

        horizontalLayout_2->addWidget(profileImg);

        horizontalSpacer_3 = new QSpacerItem(200, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 35, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        accountInput = new QLineEdit(widget_2);
        accountInput->setObjectName("accountInput");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(accountInput->sizePolicy().hasHeightForWidth());
        accountInput->setSizePolicy(sizePolicy3);
        accountInput->setMinimumSize(QSize(0, 40));
        accountInput->setMaximumSize(QSize(16777215, 16777215));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        accountInput->setFont(font1);
        accountInput->setMaxLength(11);

        verticalLayout->addWidget(accountInput);

        passwordInput = new QLineEdit(widget_2);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setMinimumSize(QSize(0, 40));
        QFont font2;
        font2.setPointSize(12);
        passwordInput->setFont(font2);
        passwordInput->setMaxLength(16);
        passwordInput->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout->addWidget(passwordInput);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        agreeBtn = new QRadioButton(widget_2);
        agreeBtn->setObjectName("agreeBtn");

        horizontalLayout_4->addWidget(agreeBtn);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 35, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        loginBtn = new QPushButton(widget_2);
        loginBtn->setObjectName("loginBtn");
        loginBtn->setMinimumSize(QSize(0, 35));
        loginBtn->setFont(font2);
        loginBtn->setCursor(QCursor(Qt::CursorShape::ForbiddenCursor));

        verticalLayout->addWidget(loginBtn);

        verticalSpacer = new QSpacerItem(20, 55, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        scanBtn = new QPushButton(widget_2);
        scanBtn->setObjectName("scanBtn");
        scanBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_3->addWidget(scanBtn);

        line = new QFrame(widget_2);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        horizontalLayout_3->addWidget(line);

        moreBtn = new QPushButton(widget_2);
        moreBtn->setObjectName("moreBtn");
        moreBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_3->addWidget(moreBtn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(widget_2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        settingBtn->setText(QString());
        minimizeBtn->setText(QString());
        closeBtn->setText(QString());
        profileImg->setText(QString());
        accountInput->setPlaceholderText(QCoreApplication::translate("Widget", "QQ\345\217\267\347\240\201/\346\211\213\346\234\272", nullptr));
        passwordInput->setPlaceholderText(QCoreApplication::translate("Widget", "\345\257\206\347\240\201", nullptr));
        agreeBtn->setText(QCoreApplication::translate("Widget", "\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\346\234\215\345\212\241\345\215\217\350\256\256\345\222\214QQ\351\232\220\347\247\201\344\277\235\346\212\244\346\214\207\345\274\225", nullptr));
        loginBtn->setText(QCoreApplication::translate("Widget", "\347\231\273\345\275\225", nullptr));
        scanBtn->setText(QCoreApplication::translate("Widget", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        moreBtn->setText(QCoreApplication::translate("Widget", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H

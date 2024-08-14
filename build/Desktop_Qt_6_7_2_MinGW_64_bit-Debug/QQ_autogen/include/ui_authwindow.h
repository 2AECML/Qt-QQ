/********************************************************************************
** Form generated from reading UI file 'authwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHWINDOW_H
#define UI_AUTHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AuthWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *settingBtn;
    QToolButton *minimizeBtn;
    QToolButton *closeBtn;
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QVBoxLayout *verticalLayout_6;
    QWidget *loginContent;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QLabel *profileImg_2;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_4;
    QLineEdit *loginAccountInput;
    QLineEdit *loginPasswordInput;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_10;
    QRadioButton *loginAgreeBtn;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_5;
    QPushButton *loginBtn;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *toRegisterBtn;
    QFrame *line_2;
    QPushButton *forgotBtn;
    QSpacerItem *horizontalSpacer_13;
    QWidget *registerPage;
    QVBoxLayout *verticalLayout_5;
    QWidget *registerContent;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_7;
    QLineEdit *registerNicknameInput;
    QLabel *nicknameHint;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *registerPasswordInput;
    QLabel *passwordHint;
    QLineEdit *registerPhoneInput;
    QLabel *phoneHint;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *registerVcodeInput;
    QPushButton *registerVcodeBtn;
    QSpacerItem *verticalSpacer_8;
    QPushButton *registerBtn;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_14;
    QPushButton *toLoginBtn;
    QSpacerItem *horizontalSpacer_15;

    void setupUi(QWidget *AuthWindow)
    {
        if (AuthWindow->objectName().isEmpty())
            AuthWindow->setObjectName("AuthWindow");
        AuthWindow->resize(300, 420);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AuthWindow->sizePolicy().hasHeightForWidth());
        AuthWindow->setSizePolicy(sizePolicy);
        AuthWindow->setMinimumSize(QSize(300, 420));
        AuthWindow->setMaximumSize(QSize(16777215, 16777215));
        AuthWindow->setSizeIncrement(QSize(0, 0));
        AuthWindow->setBaseSize(QSize(0, 0));
        AuthWindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(AuthWindow);
        verticalLayout_2->setSpacing(7);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        titleBar = new QWidget(AuthWindow);
        titleBar->setObjectName("titleBar");
        horizontalLayout = new QHBoxLayout(titleBar);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 10);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        settingBtn = new QToolButton(titleBar);
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

        minimizeBtn = new QToolButton(titleBar);
        minimizeBtn->setObjectName("minimizeBtn");
        minimizeBtn->setMinimumSize(QSize(0, 0));
        minimizeBtn->setMaximumSize(QSize(16777215, 16777215));
        minimizeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        minimizeBtn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(minimizeBtn);

        closeBtn = new QToolButton(titleBar);
        closeBtn->setObjectName("closeBtn");
        closeBtn->setMinimumSize(QSize(0, 0));
        closeBtn->setMaximumSize(QSize(16777215, 16777215));
        closeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        closeBtn->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(closeBtn);


        verticalLayout_2->addWidget(titleBar);

        stackedWidget = new QStackedWidget(AuthWindow);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setAutoFillBackground(false);
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        verticalLayout_6 = new QVBoxLayout(loginPage);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(20, 10, 20, 20);
        loginContent = new QWidget(loginPage);
        loginContent->setObjectName("loginContent");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(loginContent->sizePolicy().hasHeightForWidth());
        loginContent->setSizePolicy(sizePolicy1);
        loginContent->setMinimumSize(QSize(0, 0));
        verticalLayout_3 = new QVBoxLayout(loginContent);
        verticalLayout_3->setSpacing(10);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_8 = new QSpacerItem(200, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        profileImg_2 = new QLabel(loginContent);
        profileImg_2->setObjectName("profileImg_2");
        profileImg_2->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(profileImg_2->sizePolicy().hasHeightForWidth());
        profileImg_2->setSizePolicy(sizePolicy2);
        profileImg_2->setMinimumSize(QSize(80, 80));
        profileImg_2->setMaximumSize(QSize(80, 80));
        profileImg_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        profileImg_2->setPixmap(QPixmap(QString::fromUtf8(":/images/QQ.svg")));
        profileImg_2->setScaledContents(true);
        profileImg_2->setWordWrap(false);
        profileImg_2->setMargin(10);
        profileImg_2->setOpenExternalLinks(false);

        horizontalLayout_5->addWidget(profileImg_2);

        horizontalSpacer_9 = new QSpacerItem(200, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_5);

        verticalSpacer_4 = new QSpacerItem(20, 35, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        loginAccountInput = new QLineEdit(loginContent);
        loginAccountInput->setObjectName("loginAccountInput");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(loginAccountInput->sizePolicy().hasHeightForWidth());
        loginAccountInput->setSizePolicy(sizePolicy3);
        loginAccountInput->setMinimumSize(QSize(0, 40));
        loginAccountInput->setMaximumSize(QSize(16777215, 16777215));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        loginAccountInput->setFont(font1);
        loginAccountInput->setMaxLength(11);

        verticalLayout_3->addWidget(loginAccountInput);

        loginPasswordInput = new QLineEdit(loginContent);
        loginPasswordInput->setObjectName("loginPasswordInput");
        loginPasswordInput->setMinimumSize(QSize(0, 40));
        QFont font2;
        font2.setPointSize(12);
        loginPasswordInput->setFont(font2);
        loginPasswordInput->setMaxLength(16);
        loginPasswordInput->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_3->addWidget(loginPasswordInput);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_10);

        loginAgreeBtn = new QRadioButton(loginContent);
        loginAgreeBtn->setObjectName("loginAgreeBtn");

        horizontalLayout_6->addWidget(loginAgreeBtn);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_11);


        verticalLayout_3->addLayout(horizontalLayout_6);

        verticalSpacer_5 = new QSpacerItem(20, 35, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_5);

        loginBtn = new QPushButton(loginContent);
        loginBtn->setObjectName("loginBtn");
        loginBtn->setMinimumSize(QSize(0, 35));
        loginBtn->setFont(font2);
        loginBtn->setCursor(QCursor(Qt::CursorShape::ForbiddenCursor));

        verticalLayout_3->addWidget(loginBtn);

        verticalSpacer_6 = new QSpacerItem(20, 55, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalLayout_7->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_12);

        toRegisterBtn = new QPushButton(loginContent);
        toRegisterBtn->setObjectName("toRegisterBtn");
        toRegisterBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_7->addWidget(toRegisterBtn);

        line_2 = new QFrame(loginContent);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::VLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        horizontalLayout_7->addWidget(line_2);

        forgotBtn = new QPushButton(loginContent);
        forgotBtn->setObjectName("forgotBtn");
        forgotBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_7->addWidget(forgotBtn);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_13);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout_6->addWidget(loginContent);

        stackedWidget->addWidget(loginPage);
        registerPage = new QWidget();
        registerPage->setObjectName("registerPage");
        verticalLayout_5 = new QVBoxLayout(registerPage);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(20, 10, 20, 20);
        registerContent = new QWidget(registerPage);
        registerContent->setObjectName("registerContent");
        sizePolicy1.setHeightForWidth(registerContent->sizePolicy().hasHeightForWidth());
        registerContent->setSizePolicy(sizePolicy1);
        registerContent->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(registerContent);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_7 = new QSpacerItem(20, 50, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_7);

        registerNicknameInput = new QLineEdit(registerContent);
        registerNicknameInput->setObjectName("registerNicknameInput");
        sizePolicy3.setHeightForWidth(registerNicknameInput->sizePolicy().hasHeightForWidth());
        registerNicknameInput->setSizePolicy(sizePolicy3);
        registerNicknameInput->setMinimumSize(QSize(0, 40));
        registerNicknameInput->setMaximumSize(QSize(16777215, 16777215));
        registerNicknameInput->setFont(font1);
        registerNicknameInput->setMaxLength(24);

        verticalLayout->addWidget(registerNicknameInput);

        nicknameHint = new QLabel(registerContent);
        nicknameHint->setObjectName("nicknameHint");
        QFont font3;
        font3.setPointSize(8);
        nicknameHint->setFont(font3);
        nicknameHint->setMargin(0);
        nicknameHint->setIndent(-1);

        verticalLayout->addWidget(nicknameHint);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        registerPasswordInput = new QLineEdit(registerContent);
        registerPasswordInput->setObjectName("registerPasswordInput");
        sizePolicy3.setHeightForWidth(registerPasswordInput->sizePolicy().hasHeightForWidth());
        registerPasswordInput->setSizePolicy(sizePolicy3);
        registerPasswordInput->setMinimumSize(QSize(0, 40));
        registerPasswordInput->setMaximumSize(QSize(16777215, 16777215));
        registerPasswordInput->setFont(font1);
        registerPasswordInput->setMaxLength(16);
        registerPasswordInput->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        horizontalLayout_3->addWidget(registerPasswordInput);


        verticalLayout->addLayout(horizontalLayout_3);

        passwordHint = new QLabel(registerContent);
        passwordHint->setObjectName("passwordHint");

        verticalLayout->addWidget(passwordHint);

        registerPhoneInput = new QLineEdit(registerContent);
        registerPhoneInput->setObjectName("registerPhoneInput");
        registerPhoneInput->setMinimumSize(QSize(0, 40));
        registerPhoneInput->setFont(font2);
        registerPhoneInput->setMaxLength(11);
        registerPhoneInput->setEchoMode(QLineEdit::EchoMode::Normal);

        verticalLayout->addWidget(registerPhoneInput);

        phoneHint = new QLabel(registerContent);
        phoneHint->setObjectName("phoneHint");

        verticalLayout->addWidget(phoneHint);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        registerVcodeInput = new QLineEdit(registerContent);
        registerVcodeInput->setObjectName("registerVcodeInput");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(registerVcodeInput->sizePolicy().hasHeightForWidth());
        registerVcodeInput->setSizePolicy(sizePolicy4);
        registerVcodeInput->setMinimumSize(QSize(0, 40));
        registerVcodeInput->setFont(font2);
        registerVcodeInput->setMaxLength(6);
        registerVcodeInput->setEchoMode(QLineEdit::EchoMode::Normal);

        horizontalLayout_2->addWidget(registerVcodeInput);

        registerVcodeBtn = new QPushButton(registerContent);
        registerVcodeBtn->setObjectName("registerVcodeBtn");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(registerVcodeBtn->sizePolicy().hasHeightForWidth());
        registerVcodeBtn->setSizePolicy(sizePolicy5);
        QFont font4;
        font4.setPointSize(11);
        registerVcodeBtn->setFont(font4);
        registerVcodeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        registerVcodeBtn->setProperty("waiting", QVariant(false));

        horizontalLayout_2->addWidget(registerVcodeBtn);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_8 = new QSpacerItem(20, 60, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_8);

        registerBtn = new QPushButton(registerContent);
        registerBtn->setObjectName("registerBtn");
        registerBtn->setMinimumSize(QSize(0, 35));
        registerBtn->setFont(font2);
        registerBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        verticalLayout->addWidget(registerBtn);

        verticalSpacer_9 = new QSpacerItem(20, 55, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_14);

        toLoginBtn = new QPushButton(registerContent);
        toLoginBtn->setObjectName("toLoginBtn");
        toLoginBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        horizontalLayout_8->addWidget(toLoginBtn);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_15);


        verticalLayout->addLayout(horizontalLayout_8);


        verticalLayout_5->addWidget(registerContent);

        stackedWidget->addWidget(registerPage);

        verticalLayout_2->addWidget(stackedWidget);


        retranslateUi(AuthWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AuthWindow);
    } // setupUi

    void retranslateUi(QWidget *AuthWindow)
    {
        AuthWindow->setWindowTitle(QCoreApplication::translate("AuthWindow", "Widget", nullptr));
        settingBtn->setText(QString());
        minimizeBtn->setText(QString());
        closeBtn->setText(QString());
        profileImg_2->setText(QString());
        loginAccountInput->setPlaceholderText(QCoreApplication::translate("AuthWindow", "QQ\345\217\267\347\240\201/\346\211\213\346\234\272", nullptr));
        loginPasswordInput->setPlaceholderText(QCoreApplication::translate("AuthWindow", "\345\257\206\347\240\201", nullptr));
        loginAgreeBtn->setText(QCoreApplication::translate("AuthWindow", "\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\346\234\215\345\212\241\345\215\217\350\256\256\345\222\214QQ\351\232\220\347\247\201\344\277\235\346\212\244\346\214\207\345\274\225", nullptr));
        loginBtn->setText(QCoreApplication::translate("AuthWindow", "\347\231\273\345\275\225", nullptr));
        toRegisterBtn->setText(QCoreApplication::translate("AuthWindow", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        forgotBtn->setText(QCoreApplication::translate("AuthWindow", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        registerNicknameInput->setPlaceholderText(QCoreApplication::translate("AuthWindow", "\346\230\265\347\247\260", nullptr));
        nicknameHint->setText(QCoreApplication::translate("AuthWindow", "24\344\275\215\345\206\205\344\273\273\346\204\217\345\255\227\347\254\246\357\274\214\344\270\215\345\205\201\350\256\270\345\205\250\344\270\272\347\251\272\346\240\274", nullptr));
        registerPasswordInput->setPlaceholderText(QCoreApplication::translate("AuthWindow", "\345\257\206\347\240\201", nullptr));
        passwordHint->setText(QCoreApplication::translate("AuthWindow", "8-16\344\270\252\345\255\227\347\254\246\357\274\214\344\270\215\345\205\201\350\256\270\345\207\272\347\216\260\347\251\272\346\240\274\346\210\226\345\205\250\344\270\272\346\225\260\345\255\227", nullptr));
        registerPhoneInput->setPlaceholderText(QCoreApplication::translate("AuthWindow", "\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        phoneHint->setText(QCoreApplication::translate("AuthWindow", "11\344\275\215\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        registerVcodeInput->setPlaceholderText(QCoreApplication::translate("AuthWindow", "\351\252\214\350\257\201\347\240\201", nullptr));
        registerVcodeBtn->setText(QCoreApplication::translate("AuthWindow", "\345\217\221\351\200\201\351\252\214\350\257\201\347\240\201", nullptr));
        registerBtn->setText(QCoreApplication::translate("AuthWindow", "\346\263\250\345\206\214", nullptr));
        toLoginBtn->setText(QCoreApplication::translate("AuthWindow", "\345\211\215\345\276\200\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AuthWindow: public Ui_AuthWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHWINDOW_H

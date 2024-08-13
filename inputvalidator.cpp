#include "inputvalidator.h"
#include <QRegularExpression>

QString InputValidator::mAccountRegex = R"(^\d{5,11}$)";
QString InputValidator::mPasswordRegex = R"(^(?!\d+$)[\w!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]{8,16}$)";

bool InputValidator::isAccountValid(const QString &account) {
    // 创建正则表达式对象
    static const QRegularExpression regex(mAccountRegex);

    // 检查字符串是否匹配正则表达式
    QRegularExpressionMatch match = regex.match(account);

    if (match.hasMatch()) {
        return true;
    }

    return false;
}

bool InputValidator::isPasswordValid(const QString &password) {
    // 创建正则表达式对象
    static const QRegularExpression regex(mPasswordRegex);

    // 检查字符串是否匹配正则表达式
    QRegularExpressionMatch match = regex.match(password);

    if (match.hasMatch()) {
        return true;
    }

    return false;
}

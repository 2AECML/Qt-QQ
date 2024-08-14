#include "inputvalidator.h"
#include <QRegularExpression>

QString InputValidator::mAccountRegex = R"(^\d{5,11}$)";
QString InputValidator::mPasswordRegex = R"(^(?!\d+$)[\w!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]{8,16}$)";
QString InputValidator::mNicknameRegex = R"(^(?=.*\S).+$)";
QString InputValidator::mPhoneRegex = R"(^\d{11}$)";

bool InputValidator::isAccountValid(const QString& account) {
    return isExpressionValid(account, mAccountRegex);
}

bool InputValidator::isPasswordValid(const QString& password) {
    return isExpressionValid(password, mPasswordRegex);
}

bool InputValidator::isNicknameValid(const QString& nickname) {
    return isExpressionValid(nickname, mNicknameRegex);
}

bool InputValidator::isPhoneValid(const QString& phone) {
    return isExpressionValid(phone, mPhoneRegex);
}

bool InputValidator::isExpressionValid(const QString& expression, const QString& regex) {
    // 创建正则表达式对象
    const QRegularExpression rg(regex);

    // 检查字符串是否匹配正则表达式
    QRegularExpressionMatch match = rg.match(expression);

    // 返回验证结果
    return match.hasMatch() ? true : false;
}


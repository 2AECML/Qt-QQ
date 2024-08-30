#include "inputvalidator.h"
#include <QRegularExpression>

// 定义账户正则表达式
QString InputValidator::mAccountRegex = R"(^\d{5,11}$)"; // 账户必须是5到11位的数字

// 定义密码正则表达式
QString InputValidator::mPasswordRegex = R"(^(?!\d+$)[\w!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]{8,16}$)"; // 密码必须是8到16位的非全数字字符

// 定义昵称正则表达式
QString InputValidator::mNicknameRegex = R"(^(?=.*\S).+$)"; // 昵称必须至少包含一个非空白字符

// 定义电话正则表达式
QString InputValidator::mPhoneRegex = R"(^1\d{10}$)"; // 电话号码必须是以1开头的11位数字

// 验证账户是否合法
bool InputValidator::isAccountValid(const QString& account) {
    return isExpressionValid(account, mAccountRegex); // 调用通用验证函数
}

// 验证密码是否合法
bool InputValidator::isPasswordValid(const QString& password) {
    return isExpressionValid(password, mPasswordRegex); // 调用通用验证函数
}

// 验证昵称是否合法
bool InputValidator::isNicknameValid(const QString& nickname) {
    return isExpressionValid(nickname, mNicknameRegex); // 调用通用验证函数
}

// 验证电话号码是否合法
bool InputValidator::isPhoneValid(const QString& phone) {
    return isExpressionValid(phone, mPhoneRegex); // 调用通用验证函数
}

// 使用正则表达式验证字符串是否符合要求
bool InputValidator::isExpressionValid(const QString& expression, const QString& regex) {
    // 创建正则表达式对象
    const QRegularExpression rg(regex);

    // 检查字符串是否匹配正则表达式
    QRegularExpressionMatch match = rg.match(expression);

    // 返回验证结果
    return match.hasMatch(); // 如果匹配，则返回 true，否则返回 false
}

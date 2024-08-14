#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>

class InputValidator {
public:
    static bool isAccountValid(const QString& account);
    static bool isPasswordValid(const QString& password);
    static bool isNicknameValid(const QString& nickname);
    static bool isPhoneValid(const QString& phone);

private:
    static bool isExpressionValid(const QString& expression, const QString& regex);

private:
    static QString mAccountRegex;
    static QString mPasswordRegex;
    static QString mNicknameRegex;
    static QString mPhoneRegex;
};

#endif // INPUTVALIDATOR_H

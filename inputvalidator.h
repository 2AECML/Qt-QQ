#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>

class InputValidator
{
public:
    static bool isAccountValid(const QString& account);
    static bool isPasswordValid(const QString& password);

private:
    static QString mAccountRegex;
    static QString mPasswordRegex;
};

#endif // INPUTVALIDATOR_H

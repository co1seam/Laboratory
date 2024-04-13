// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef LOGOUTPUT_H
#define LOGOUTPUT_H

#include <QString>
#include <QTextStream>

/**
 * @todo write docs
 */
class LogOutput
{
public:
    virtual ~LogOutput() {}
    virtual void output(const QString &message) = 0;
};

#endif // LOGOUTPUT_H

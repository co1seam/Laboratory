// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef LOGOUTPUTCONSOLE_H
#define LOGOUTPUTCONSOLE_H

#include "logoutput.h"

/**
 * @todo write docs
 */
class LogOutputConsole : public LogOutput
{
    void output(const QString& message) override;
};

#endif // LOGOUTPUTCONSOLE_H

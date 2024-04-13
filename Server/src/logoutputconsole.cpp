// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "logoutputconsole.h"

void LogOutputConsole::output(const QString& message)
{
    QTextStream(stdout) << message << Qt::endl;
}

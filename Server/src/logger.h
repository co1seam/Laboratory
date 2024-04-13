// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QScopedPointer>
#include "logoutput.h"
#include "logoutputconsole.h"
#include "logoutputfile.h"

/**
 * @todo write docs
 */
class Logger
{
public:
    enum class Level{
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    Logger(LogOutput* backend) : output_strategy(backend) {}

    virtual ~Logger() {}

    /**
     *
     */
    void log(const QString message, Level level);

private:
    QScopedPointer<LogOutput> output_strategy;

    static QString levelToString(Level level);
    static QString getCurrentTime();
};

#endif // LOGGER_H

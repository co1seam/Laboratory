// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "logger.h"

void Logger::log(const QString message, Level level = Level::INFO)
{
    QString current_time = getCurrentTime();
    QString level_string = levelToString(level);
    output_strategy->output(QString("[%1][%2]: %3").arg(current_time, level_string, message));
}


QString Logger::levelToString(Level level)
{
    switch(level){
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

QString Logger::getCurrentTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}


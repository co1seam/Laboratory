// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "logger.h"

Logger::Logger(){
    start();
}

void Logger::run()
{
    while(true){
        LogMessage log_message;
        {
            QMutexLocker locker(&mutex);
            if(log_queue.isEmpty()){
                if(exit_flag)
                    break;
                condition.wait(&mutex);
                continue;
            }
        }
    }

    QString time = QTime::currentTime().toString("HH:mm:ss");
}


QString Logger::LogLevelToString(LogLevel level) const{
    switch(level){
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::SetLogLevel(LogLevel level){
    QMutexLocker locker(&mutex);
    current_log_level = level;
}

void Logger::Log(LogLevel level, const QString& service, const QString& message){
    QMutexLocker locker(&mutex);
    if(level >= current_log_level){
        log_queue.enqueue({level, service, message});
        condition.wakeOne();
    }
}




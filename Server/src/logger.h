// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef LOGGER_H
#define LOGGER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QString>
#include <QTextStream>
#include "loglevel.h"

/**
 * @todo write docs
 */
class Logger : QThread
{
  Q_OBJECT

public:
  Logger();
  QString LogLevelToString(LogLevel level) const;
  void SetLogLevel(LogLevel level);
  void Log(LogLevel level, const QString& service, const QString& message);

private:
  struct LogMessage{
    LogLevel level;
    QString service;
    QString message;
  };

  QQueue<LogMessage> log_queue;
  QMutex mutex;
  QWaitCondition condition;
  bool exit_flag = false;
  LogLevel current_log_level = LogLevel::Info;

protected:
  void run() override;
};

#endif // LOGGER_H

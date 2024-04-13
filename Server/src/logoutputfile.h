// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef LOGOUTPUTFILE_H
#define LOGOUTPUTFILE_H

#include <QFile>
#include "logoutput.h"

/**
 * @todo write docs
 */
class LogOutputFile : public LogOutput
{
public:
    LogOutputFile(const QString& filename);
    ~LogOutputFile();
    void output(const QString& message) override;
private:
    QFile file_stream;
};

#endif // LOGOUTPUTFILE_H

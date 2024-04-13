// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "logoutputfile.h"

LogOutputFile::LogOutputFile(const QString& filename) : file_stream(filename)
{
    file_stream.open(QIODevice::Append | QIODevice::Text);
}

LogOutputFile::~LogOutputFile()
{
    file_stream.close();
}

void LogOutputFile::output(const QString& message)
{
    if(file_stream.isOpen()){
        QTextStream out(&file_stream);
        out << message <<Qt::endl;
    }
}

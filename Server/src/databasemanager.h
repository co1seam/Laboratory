// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QSqlRecord>

#include "logger.h"

/**
 * @todo write docs
 */
class DatabaseManager
{
public:
    /**
     * Constructor.
     *
     */
    DatabaseManager(const QString& driver,
                    const QString& hostname,
                    int port,
                    const QString& database_name,
                    const QString& username,
                    const QString& password,
                    Logger *logger
    );

    ~DatabaseManager();

    bool open();

    void close();


    /**
     *
     */
    bool executeStoredProcedure(const QString& procedure_name, const QVariantList& parameters);

    QByteArray executeSqlFunction(const QString& function_name);

private:
    QSqlDatabase db;
    QScopedPointer<Logger> console_logger;
};

#endif // DATABASEMANAGER_H

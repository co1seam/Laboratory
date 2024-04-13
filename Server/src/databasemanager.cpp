// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString& driver,
                                 const QString& hostname,
                                 int port,
                                 const QString& database_name,
                                 const QString& username,
                                 const QString& password,
                                 Logger* logger
                                 ) : console_logger(logger)
{
    db = QSqlDatabase::addDatabase(driver);
    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(database_name);
    db.setUserName(username);
    db.setPassword(password);
}

DatabaseManager::~DatabaseManager() = default;

bool DatabaseManager::open()
{
    if(!db.open()){
        console_logger->log(QString("Connection with database failed: \033[1m%1\033[0m").arg(db.lastError().text()), Logger::Level::DEBUG);
        return false;
    }
    console_logger->log(QString("Connection with database success: \033[1m%1\033[0m").arg(db.databaseName()), Logger::Level::DEBUG);

    return true;
}


void DatabaseManager::close()
{
    if(db.isOpen()){
        db.close();
    }
}

bool DatabaseManager::executeStoredProcedure(const QString& procedure_name, const QVariantList& parameters = QVariantList())
{
    open();

    QSqlQuery query(db);
    QString call_string = QString("CALL %1(").arg(procedure_name);
    for(qsizetype i = 0; i < parameters.size(); ++i){
        call_string += (i == 0 ? "?" : ", ?");
    }
    call_string += ")";

    for(qsizetype i = 0; i < parameters.size(); ++i){
        query.bindValue(i, parameters[i]);
    }

    if(!query.exec()){
        console_logger->log(QString("Failed to execute stored procedure: \033[1m%1\033[0m").arg(query.lastError().text()), Logger::Level::ERROR);
        return false;
    }

    close();

    return true;
}

QByteArray DatabaseManager::executeSqlFunction(const QString& function_name)
{
    if(!open()){
        console_logger->log(QString("Database failed open"), Logger::Level::ERROR);
        return QByteArray();
    }

    QSqlQuery query;
    QString call_string = QString("SELECT * FROM %1();").arg(function_name);

    if(!query.exec(call_string)){
        console_logger->log(QString("Failed to execute function: \033[1m%1\033[0m").arg(query.lastError().text()), Logger::Level::ERROR);
        close();
        return QByteArray();
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    out << qint32(0);

    qint32 block_size = 0;
    if(query.first()){

        qint32 column_count = query.record().count();
        out << column_count;

        for(qsizetype i = 0; i < column_count; ++i){
            out << query.record().fieldName(i);
            block_size += query.record().fieldName(i).size();
        }

        do{
            for(qsizetype i = 0; i < column_count; ++i){
                out << query.value(i).toString();
                block_size += query.value(i).toString().size();
            }
        } while (query.next());
    }


    out.device()->seek(0);
    out << qint32(block.size() - block_size);

    close();
    return block;
}

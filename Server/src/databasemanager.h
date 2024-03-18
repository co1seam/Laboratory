// SPDX-FileCopyrightText: 2024 Константин <email>
// SPDX-License-Identifier: MIT

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

/**
 * @todo write docs
 */
class DatabaseManager
{
protected:
    QSqlDatabase m_db;
public:
    /**
     * Default constructor
     */
    DatabaseManager(){
        m_db = QSqlDatabase::addDatabase("QPSQL");
        m_db.setHostName(hostname);
        m_db.setDatabaseName(filename);
        m_db.setUserName(username);
        m_db.setPassword(password);
        if(!m_db.open()){
            qDebug() << QString("Ошибка при открытии базы данных. %1");
        }
    }
    bool ConnectToDatabase();
    bool InsertIntoTable();
    void SelectFrom();

private:
    const QString hostname = "localhost";
    const QString filename = "laboratory";
    const QString username = "admin";
    const QString password = "root";
    void CloseDatabase();
};

#endif // DATABASEMANAGER_H

/*
 * Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 https://github.com/klappdev
 *
 * Permission is hereby  granted, free of charge, to any  person obtaining a copy
 * of this software and associated  documentation files (the "Software"), to deal
 * in the Software  without restriction, including without  limitation the rights
 * to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 * copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 * IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 * FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 * AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 * LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "db/TaskDao.hpp"

static constexpr const char* const TAG = "[TaskDao] ";
static constexpr const char* const DB_NAME = "task_connection";    //FIXME: rename - account
static constexpr const char* const DB_FILE = "task.sqlite";        //FIXME: rename - notiz.sqlite

TaskDao::TaskDao()
    : mDatabase(createConnection())
    , mSqlQuery(QSqlQuery(mDatabase))
    , mSqlRecord(mSqlQuery.record()) {
    createTable();
}

TaskDao::~TaskDao() {
    mDatabase.close();

    QSqlDatabase::removeDatabase(DB_NAME);
}

auto TaskDao::getInstance() -> TaskDao& {
    static TaskDao instance;

    return instance;
}

auto TaskDao::createConnection() -> QSqlDatabase {
    QDir databaseFile(QString::fromStdString(DB_FILE));

    auto database = QSqlDatabase::addDatabase("QSQLITE", DB_NAME);
    database.setDatabaseName(databaseFile.absolutePath());

    if (!database.open()) {
        qCritical() << TAG << "Can't open db: " << database.lastError() << endl;
    } else {
        qInfo() << TAG << "DB connected!" << endl;
    }

    database.exec("PRAGMA locking_mode = EXCLUSIVE");

    return database;
}

void TaskDao::createTable() {
    if (!mSqlQuery.exec(R"xxx(CREATE TABLE IF NOT EXISTS task (
                                 id INTEGER PRIMARY KEY NOT NULL,
                                 id_account INTEGER NOT NULL,
                                 favorited INTEGER NOT NULL,
                                 completed INTEGER NOT NULL,
                                 timestamp VARCHAR(100),
                                 name VARCHAR(255),
                                 description VARCHAR(255));
                        )xxx")) {
        qCritical() << TAG << "Table task was not created!" << mDatabase.lastError() << endl;
    } else {
        qInfo() << TAG << "Table task was created!" << endl;
    }

    mSqlQuery.executedQuery();
}

bool TaskDao::checkIfExists(const Task& task) {
    bool success = false;

    mSqlQuery.prepare("SELECT COUNT(*) FROM task WHERE id=?");
    mSqlQuery.addBindValue(task.id);

    if (!mSqlQuery.exec() || !mSqlQuery.first()) {
       qCritical() << TAG << "check if exists task failed:  " << mSqlQuery.lastError();
    } else if (mSqlQuery.value(0) == 0) {
        success = false;
    } else {
       success = true;
    }

    return success;
}

void TaskDao::init(Task& task) {
    task.id = mSqlQuery.value(mSqlRecord.indexOf("id")).toInt();
    task.idAccount = mSqlQuery.value(mSqlRecord.indexOf("id_account")).toInt();

    task.completed = mSqlQuery.value(mSqlRecord.indexOf("completed")).toInt();
    task.favorited = mSqlQuery.value(mSqlRecord.indexOf("favorited")).toInt();

    task.timestamp = mSqlQuery.value(mSqlRecord.indexOf("timestamp")).toString();
    task.name = mSqlQuery.value(mSqlRecord.indexOf("name")).toString();
    task.description = mSqlQuery.value(mSqlRecord.indexOf("description")).toString();
}

void TaskDao::reset() {
    bool success = false;

    if (mSqlQuery.exec("DELETE FROM task")) {
       success = true;
    } else {
        qCritical() << TAG << "delete task error:  " << mSqlQuery.lastError();
    }
}

bool TaskDao::add(const Task& task) {
    bool success = false;

    mSqlQuery.prepare(R"xxx(INSERT INTO task (
                                id_account,
                                favorited,
                                completed,
                                timestamp,
                                name,
                                description)
                            VALUES (?, ?, ?, ?, ?, ?);
                      )xxx");

    mSqlQuery.addBindValue(task.idAccount);
    mSqlQuery.addBindValue(task.favorited);
    mSqlQuery.addBindValue(task.completed);
    mSqlQuery.addBindValue(task.timestamp);
    mSqlQuery.addBindValue(task.name);
    mSqlQuery.addBindValue(task.description);

    if (mSqlQuery.exec()) {
        success = true;
    } else {
        qCritical() << TAG << "add task error:  " << mSqlQuery.lastError();
    }

    return success;
}

bool TaskDao::update(const Task& task) {
    bool success = false;

    mSqlQuery.prepare(R"xxx(UPDATE task SET id_account=?,
                                            completed=?,
                                            favorited=?,
                                            timestamp=?,
                                            name=?, description=?
                            WHERE id=?;
                      )xxx");

    mSqlQuery.addBindValue(task.idAccount);
    mSqlQuery.addBindValue(task.completed);
    mSqlQuery.addBindValue(task.favorited);
    mSqlQuery.addBindValue(task.timestamp);
    mSqlQuery.addBindValue(task.name);
    mSqlQuery.addBindValue(task.description);
    mSqlQuery.addBindValue(task.id);

    if (mSqlQuery.exec()) {
        success = true;
    } else {
        qCritical() << TAG << "update task error:  " << mSqlQuery.lastError();
    }

    return success;
}

bool TaskDao::remove(int id) {
    bool success = false;

    mSqlQuery.prepare("DELETE FROM task WHERE id=?;");
    mSqlQuery.addBindValue(id);

    if (mSqlQuery.exec()) {
       success = true;
    } else {
        qCritical() << TAG << "delete task error:  " << mSqlQuery.lastError();
    }

    return success;
}

auto TaskDao::get(qint32 id) -> Task {
    Task task = {};

    mSqlQuery.prepare("SELECT * FROM task WHERE id=?;");
    mSqlQuery.addBindValue(id);

    if (!mSqlQuery.exec()) {
       qCritical() << TAG << "select concrete task error:  " << mSqlQuery.lastError();
    }

    mSqlRecord = mSqlQuery.record();

    if (mSqlQuery.next()) {
        init(task);
    }

    return task;
}

auto TaskDao::getAll() -> QList<Task> {
    QList<Task> list;
    Task task = {};

    if (!mSqlQuery.exec("SELECT * FROM task;")) {
       qCritical() << TAG << "select all task error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

auto TaskDao::getAccountAll(qint32 idAccount) -> QList<Task> {
    QList<Task> list;
    Task task = {};

    mSqlQuery.prepare("SELECT * FROM task WHERE id_account=?;");
    mSqlQuery.addBindValue(idAccount);

    if (!mSqlQuery.exec()) {
       qCritical() << "get account tasks error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

auto TaskDao::getAllFavorite() -> QList<Task> {
    QList<Task> list;
    Task task = {};

    mSqlQuery.prepare("SELECT * FROM task WHERE favorited=?;");
    mSqlQuery.addBindValue(1);

    if (!mSqlQuery.exec()) {
       qCritical() << "select all task by favorited error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

auto TaskDao::getAllComplete() -> QList<Task> {
    QList<Task> list;
    Task task = {};

    mSqlQuery.prepare("SELECT * FROM task WHERE completed=?;");
    mSqlQuery.addBindValue(1);

    if (!mSqlQuery.exec()) {
       qCritical() << "select all task by completed error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

auto TaskDao::search(const QString& description) -> QList<Task> {
    QList<Task> list;
    Task task = {};

    mSqlQuery.prepare("SELECT * FROM task WHERE description LIKE ?;");
    mSqlQuery.addBindValue("%" + description + "%");

    if (!mSqlQuery.exec()) {
       qCritical() << "search tasks error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

auto TaskDao::search(const QDate& date) -> QList<Task> {
    QList<Task> list;
    Task task = {};

    mSqlQuery.prepare("SELECT * FROM task WHERE timestamp=?;");
    mSqlQuery.addBindValue(date.toString("dd/MM/yyyy"));

    if (!mSqlQuery.exec()) {
       qCritical() << "search by timestamp error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

auto TaskDao::sort(bool asc) -> QList<Task> {
    QList<Task> list;
    Task task = {};

    const QString ascending = asc ? "ASC" : "DESC";

    if (!mSqlQuery.exec("SELECT * FROM task ORDER BY name " + ascending + ";")) {
       qCritical() << TAG << "sort tasks error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(task);
        list.append(task);
    }

    return list;
}

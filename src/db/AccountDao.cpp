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

#include "db/AccountDao.hpp"

static constexpr const char* const TAG = "[AccountDao] ";
static constexpr const char* const DB_NAME = "account_connection"; //FIXME: rename - account
static constexpr const char* const DB_FILE = "task.sqlite";        //FIXME: rename - notiz.sqlite

AccountDao::AccountDao()
    : mDatabase(createConnection())
    , mSqlQuery(QSqlQuery(mDatabase))
    , mSqlRecord(mSqlQuery.record()) {
    createTable();
}

AccountDao::~AccountDao() {
    mDatabase.close();

    QSqlDatabase::removeDatabase(DB_NAME);
}

AccountDao& AccountDao::getInstance() {
    static AccountDao instance;

    return instance;
}

auto AccountDao::createConnection() -> QSqlDatabase {
    QDir databaseFile(QString::fromStdString(DB_FILE));

    auto database = QSqlDatabase::addDatabase("QSQLITE", DB_NAME);
    mDatabase.setDatabaseName(databaseFile.absolutePath());

    if (!mDatabase.open()) {
        qCritical() << TAG << "Can't open db: " << mDatabase.lastError() << endl;
    } else {
        qInfo() << TAG << "DB connected!" << endl;
    }

    return database;
}

void AccountDao::createTable() {
    if (!mSqlQuery.exec(R"xxx(CREATE TABLE IF NOT EXISTS account (
                                id INTEGER PRIMARY KEY NOT NULL,
                                login VARCHAR(255),
                                password VARCHAR(255));
                        )xxx")) {
        qCritical() << TAG << "Table account was not created!" << mDatabase.lastError() << endl;
    } else {
        qInfo() << TAG << "Table account was created!" << endl;
    }

    mSqlQuery.executedQuery();
}

bool AccountDao::checkIfExists(const Account& account) {
    bool success = false;

    mSqlQuery.prepare("SELECT COUNT(*) FROM account WHERE id=?");
    mSqlQuery.addBindValue(account.id);

    if (!mSqlQuery.exec() || !mSqlQuery.first()) {
       qCritical() << TAG << "check if exists account failed:  " << mSqlQuery.lastError();
    } else if (mSqlQuery.value(0) == 0) {
        success = false;
    } else {
       success = true;
    }

    return success;
}

bool AccountDao::checkIfExists(const QString& login, const QString& password) {
    bool success = false;

    mSqlQuery.prepare("SELECT COUNT(*) FROM account WHERE login=? AND password=?");
    mSqlQuery.addBindValue(login);
    mSqlQuery.addBindValue(password);

    if (!mSqlQuery.exec() || !mSqlQuery.first()) {
       qCritical() << TAG << "check if exists account failed:  " << mSqlQuery.lastError();
    } else if (mSqlQuery.value(0) == 0) {
        success = false;
    } else {
       success = true;
    }

    mSqlQuery.finish();

    return success;
}

void AccountDao::init(Account& account) {
    account.id = mSqlQuery.value(mSqlRecord.indexOf("id")).toInt();

    account.login = mSqlQuery.value(mSqlRecord.indexOf("login")).toString();
    account.password = mSqlQuery.value(mSqlRecord.indexOf("password")).toString();
}

bool AccountDao::add(const Account& account) {
    bool success = false;

    mSqlQuery.prepare(R"xxx(INSERT OR IGNORE INTO account (
                                login,
                                password)
                            VALUES (?, ?);
                      )xxx");
    mSqlQuery.addBindValue(account.login);
    mSqlQuery.addBindValue(account.password);

    if (mSqlQuery.exec()) {
        success = true;
    } else {
        qCritical() << TAG << "add account error:  " << mSqlQuery.lastError();
    }

    return success;
}

bool AccountDao::update(const Account& account) {
    bool success = false;

    mSqlQuery.prepare(R"xxx(UPDATE account SET login=?,
                                               password=?
                            WHERE id=?;
                      )xxx");
    mSqlQuery.addBindValue(account.login);
    mSqlQuery.addBindValue(account.password);
    mSqlQuery.addBindValue(account.id);

    if (mSqlQuery.exec()) {
        success = true;
    } else {
        qCritical() << TAG << "update account error:  " << mSqlQuery.lastError();
    }

    return success;
}

bool AccountDao::remove(int id) {
    bool success = false;

    mSqlQuery.prepare("DELETE FROM account WHERE id=?;");
    mSqlQuery.addBindValue(id);

    if (mSqlQuery.exec()) {
       success = true;
    } else {
        qCritical() << TAG << "delete account error:  " << mSqlQuery.lastError();
    }

    return success;
}

auto AccountDao::get(qint32 id) -> Account {
    Account account = {};

    mSqlQuery.prepare("SELECT * FROM account WHERE id=?;");
    mSqlQuery.addBindValue(id);

    if (!mSqlQuery.exec()) {
       qCritical() << TAG << "select concrette account error:  " << mSqlQuery.lastError();
    }

    mSqlRecord = mSqlQuery.record();

    if (mSqlQuery.next()) {
        init(account);
    }

    return account;
}

auto AccountDao::getAll() -> QList<Account> {
    QList<Account> list;
    Account account;

    if (!mSqlQuery.exec("SELECT * FROM account;")) {
       qCritical() << TAG << "select all account error:  " << mSqlQuery.lastError() << endl;
    }

    mSqlRecord = mSqlQuery.record();

    while (mSqlQuery.next()) {
        init(account);
        list.append(account);
    }

    return list;
}

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

#pragma once

#include <QtSql>

#include "common/Account.hpp"

class AccountDao final {
public:
    AccountDao(const AccountDao&) = delete;
    AccountDao& operator=(AccountDao&) = delete;

    static auto getInstance() -> AccountDao&;

    bool checkIfExists(const Account& account);
    bool checkIfExists(const QString& login, const QString& password);

    bool add(const Account& account);
    bool update(const Account& account);
    bool remove(int id);

    auto get(qint32 id) -> Account;
    auto getAll() -> QList<Account>;

private:
    AccountDao();
    ~AccountDao();

    auto createConnection() -> QSqlDatabase;
    void createTable();

    void init(Account& account);

    QSqlDatabase mDatabase;
    QSqlQuery    mSqlQuery;
    QSqlRecord   mSqlRecord;
};

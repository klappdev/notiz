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

#include "common/Task.hpp"

class TaskDao final {
public:
    TaskDao(const TaskDao&) = delete;
    TaskDao& operator=(TaskDao&) = delete;

    static auto getInstance() -> TaskDao&;

    void reset();

    bool checkIfExists(const Task& task);

    bool add(const Task& task);
    bool update(const Task& task);
    bool remove(int id);

    auto get(qint32 id) -> Task;
    auto getAll() -> QList<Task>;
    auto getAllFavorite() -> QList<Task>;
    auto getAllComplete() -> QList<Task>;
    auto getAccountAll(qint32 idAccount) -> QList<Task>;

    auto search(const QString& text) -> QList<Task>;
    auto search(const QDate& date) -> QList<Task>;
    auto sort(bool asc) -> QList<Task>;

private:
    TaskDao();
    ~TaskDao();

    auto createConnection() -> QSqlDatabase;
    void createTable();

    void init(Task& task);

    QSqlDatabase mDatabase;
    QSqlQuery    mSqlQuery;
    QSqlRecord   mSqlRecord;
};

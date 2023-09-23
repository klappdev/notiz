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

#include <QAbstractTableModel>
#include <QModelIndex>

#include <QHash>
#include <QList>
#include <QVariant>

#include "common/Task.hpp"

class TaskModel final : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit TaskModel(QObject* parent = nullptr);
    ~TaskModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &index = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setTasks(QList<Task>&& list);
    const QList<Task>& getTasks() const;

private:
    QVariant obtainIconTask(int row, int column) const;
    QVariant obtainDetailTask(int row, int column) const;

    QStringList prepareNameHeaders() const;

    QHash<QModelIndex, QVariant> mHashData;
    QList<Task> mTasks;

    const QStringList mHeadersTable;
};

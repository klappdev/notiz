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

#include "model/TaskModel.hpp"

#include <QtAlgorithms>
#include <QIcon>

static constexpr const char* const TAG =  "[TaskModel] ";

static constexpr int ID_COLUMN = 0;
static constexpr int TIMESTAMP_COLUMN = 1;
static constexpr int FAVORITED_COLUMN = 2;
static constexpr int COMPLETED_COLUMN = 3;
static constexpr int NAME_COLUMN = 4;
static constexpr int DESCRIPTION_COLUMN = 5;

static constexpr const char* const TIMESTAMP_DEFAULT_VALUE = "01.01.2000_00:00:00";
static constexpr const char* const NAME_DEFAULT_VALUE = "<no_name>";
static constexpr const char* const DESCRIPTION_DEFAULT_VALUE = "<no_description>";

TaskModel::TaskModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mHeadersTable(prepareNameHeaders()) {
}

TaskModel::~TaskModel() {}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const {
    QVariant currentColumn;

    if (role != Qt::DisplayRole) {
        return currentColumn;
    }

    if (orientation == Qt::Horizontal) {
        currentColumn = mHeadersTable[section];
    }

    return currentColumn;
}

int TaskModel::rowCount(const QModelIndex&) const {
    return mTasks.size();
}

int TaskModel::columnCount(const QModelIndex&) const {
    return mHeadersTable.size();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const {
    QVariant value;

    if (!index.isValid()) {
        qCritical() << TAG << "Index is not valied: [" << index.column() << ":" << index.row() << "]";
        return value;
    }

    switch (role) {
    case Qt::TextAlignmentRole:
        return int(Qt::AlignLeft | Qt::AlignVCenter);

        break;
    case Qt::DecorationRole:
        value = obtainIconTask(index.row(), index.column());

        return mHashData.value(index, value);
        break;
    case Qt::DisplayRole:
        value = obtainDetailTask(index.row(), index.column());

        return mHashData.value(index, value);
        break;
    }

    return value;
}

bool TaskModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (index.isValid() &&  role == Qt::EditRole) {
        mHashData[index] = value;

        emit dataChanged(index, index);

        return true;
    }

    return false;      
}

bool TaskModel::removeRows(int position, int rows, const QModelIndex& index) {
    Q_UNUSED(index);

    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        mTasks.removeAt(position);
    }

    endRemoveRows();

    return true;
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}

QVariant TaskModel::obtainIconTask(int row, int column) const {
    QVariant value;

    if (column == FAVORITED_COLUMN) {
        const bool favorited = mTasks.at(row).favorited;

        if (favorited) {
            value = QIcon(":res/image/type/favorite.png");
        } else {
            value = QIcon(":res/image/type/unfavorite.png");
        }
    } else if (column == COMPLETED_COLUMN) {
        const bool completed = mTasks.at(row).completed;

        if (completed) {
            value = QIcon(":res/image/type/complete.png");
        } else {
            value = QIcon(":res/image/type/uncomplete.png");
        }
    }

    return value;
}

QVariant TaskModel::obtainDetailTask(int row, int column) const {
    QVariant value;

    switch (column) {
    case ID_COLUMN: {
        value = mTasks.at(row).id;

        break;
    }
    case TIMESTAMP_COLUMN: {
        const QString timestamp = mTasks.at(row).timestamp;
        value = !timestamp.isEmpty() ? timestamp : TIMESTAMP_DEFAULT_VALUE;

        break;
    }
    case NAME_COLUMN: {
        const QString name = mTasks.at(row).name;
        value = !name.isEmpty() ? name : NAME_DEFAULT_VALUE;

        break;
        }
    case DESCRIPTION_COLUMN: {
        QString description = mTasks.at(row).description;
        value = !description.isEmpty() ? description : DESCRIPTION_DEFAULT_VALUE;

        break;
    }
    default:
        //qCritical() << TAG << "Unknown column";
        break;
    }

    return value;
}

QStringList TaskModel::prepareNameHeaders() const {
    return {
        tr("№"), tr("Date"),
        tr("Choose"), tr("Finished"),
        tr("Name"), tr("Comment")
    };
}

void TaskModel::setTasks(QList<Task>&& list) {
    this->mTasks = list;

    beginResetModel();

    for (int row = 0; row < rowCount(); ++row) {
        for (int column = 0; column < columnCount(); ++column) {
            const QModelIndex& currentIndex = index(row, column);

            if (column == FAVORITED_COLUMN || column == COMPLETED_COLUMN) {
                mHashData[currentIndex] = obtainIconTask(row, column);
            } else {
                mHashData[currentIndex] = obtainDetailTask(row, column);
            }
        }
    }

    endResetModel();
}

const QList<Task>& TaskModel::getTasks() const {
    return mTasks;
}


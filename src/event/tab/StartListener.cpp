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

#include "event/tab/StartListener.hpp"
#include "db/TaskDao.hpp"

#include "ui/dialog/AddTaskDialog.hpp"
#include "ui/dialog/EditTaskDialog.hpp"

#include "ui/main/MainWindow.hpp"
#include "ui/util/UiUtil.hpp"

static constexpr const char* const TAG = "[StartListener] ";
static constexpr int ID_COLUMN = 0;
static constexpr int TIMESTAMP_COLUMN = 1;
static constexpr int COMPLETED_COLUMN = 2;
static constexpr int FAVORITED_COLUMN = 3;
static constexpr int NAME_COLUMN = 4;
static constexpr int DESCRIPTION_COLUMN = 5;

StartListener::StartListener(StartPanel* panel)
    : mPanel(panel) {
}

StartListener::~StartListener() {}

void StartListener::showTask(const QPoint& position) {
    const QPoint newPosition = mPanel->getTable()->mapToGlobal(position);

    mPanel->getMenu()->exec(newPosition);
}

void StartListener::showAddTask() {
    AddTaskDialog dialog(tr("Add task"));
    dialog.setParent(mPanel);

    if (dialog.exec() == QDialog::Accepted) {
        qInfo() << TAG << "Dialog was closed!" << endl;
    }
}

void StartListener::showEditTask() {
    const QString format = "dd-MM-yyyy_hh:mm:ss";
    const QModelIndex index = mPanel->getTable()->selectionModel()->currentIndex();

    if (index.isValid()) {
        EditTaskDialog dialog(tr("Edit task"));
        dialog.setParent(mPanel);

        const qint32 number  = index.sibling(index.row(), ID_COLUMN).data().toInt();
        const bool completed = index.sibling(index.row(), COMPLETED_COLUMN).data().toInt();
        const bool favorited = index.sibling(index.row(), FAVORITED_COLUMN).data().toInt();

        const QString timestamp = index.sibling(index.row(), TIMESTAMP_COLUMN).data().toString();
        const QString name = index.sibling(index.row(), NAME_COLUMN).data().toString();
        const QString description = index.sibling(index.row(), DESCRIPTION_COLUMN).data().toString();

        dialog.setNumberRow(number);
        dialog.getCompleteCheckBox()->setChecked(completed);
        dialog.getFavoriteCheckBox()->setChecked(favorited);
        dialog.getTimestampEdit()->setDateTime(QDateTime::fromString(timestamp, format));
        dialog.getNameEdit()->setText(name);
        dialog.getDescriptionEdit()->setText(description);

        if (dialog.exec() == QDialog::Accepted) {
            qInfo() << TAG << "Dialog was closed!" << endl;
        }
    } else {
        UiUtil::showInfoMessage(tr("Edit task"),
                                tr("Line was not choosed!"));
    }
}

void StartListener::showDeleteTask() {
   const QModelIndex index = mPanel->getTable()->selectionModel()->currentIndex();

    if (index.isValid()) {
        const qint32 number  = index.sibling(index.row(), ID_COLUMN).data().toInt();
        TaskDao::getInstance().remove(number);

        mPanel->updateTable();

        UiUtil::showInfoMessage(tr("Delete task"),
                                tr("Task was deleted success!"));
    } else {
        UiUtil::showErrorMessage(tr("Delete task"),
                                 tr("Line was not choosed!"));
    }
}

void StartListener::showSimilarTask() {
    const QModelIndex index = mPanel->getTable()->selectionModel()->currentIndex();

    if (index.isValid()) {
        auto& taskDao = TaskDao::getInstance();

        const qint32 number = index.sibling(index.row(), ID_COLUMN).data().toInt();

        const Task savedTask = taskDao.get(number);
        const Task similarTask = {
            .id = 0,
            .idAccount = 1,
            .completed = savedTask.completed,
            .favorited = savedTask.favorited,
            .timestamp = savedTask.timestamp,
            .name = savedTask.name,
            .description = savedTask.description
        };

        taskDao.add(similarTask);

        UiUtil::showInfoMessage(tr("Duplicate task"),
                                tr("Task was duplicated success!"));
        mPanel->updateTable();
    } else {
        UiUtil::showErrorMessage(tr("Duplicate task"),
                                 tr("Line was not choosed!"));
    }
}

void StartListener::showSearchTask() {
    const QString text = mPanel->getSearcheEdit()->text();

    if (!text.isEmpty()) {
        auto& taskDao = TaskDao::getInstance();

        mPanel->updateTable(taskDao.search(text));
    } else {
        UiUtil::showErrorMessage(tr("Search tasks"),
                                 tr("Line was not input!"));
        mPanel->updateTable();
    }
}

void StartListener::sortAscTask(bool asc) {
    Q_UNUSED(asc);

    auto& taskDao = TaskDao::getInstance();
    QList<Task> tasks = taskDao.sort(true);

    if (!tasks.empty()) {
        mPanel->updateTable(std::move(tasks));
    } else {
        UiUtil::showErrorMessage(tr("Sort tasks"),
                                 tr("List tasks are empty!"));
    }
}

void StartListener::sortDescTask(bool asc) {
    Q_UNUSED(asc);

    auto& taskDao = TaskDao::getInstance();
    QList<Task> tasks = taskDao.sort(false);

    if (!tasks.empty()) {
        mPanel->updateTable(std::move(tasks));
    } else {
        UiUtil::showErrorMessage(tr("Sort tasks"),
                                 tr("List tasks are empty!"));
    }
}

void StartListener::searchTodayTask(bool checked) {
    Q_UNUSED(checked);

    auto& taskDao = TaskDao::getInstance();
    QList<Task> tasks = taskDao.search(QDate::currentDate());

    if (!tasks.empty()) {
        mPanel->updateTable(std::move(tasks));
    } else {
        UiUtil::showErrorMessage(tr("Search tasks"),
                                 tr("List tasks are empty!"));
    }
}

void StartListener::sortCalendarTask(bool checked) {
    Q_UNUSED(checked);

    auto* mainWindow = static_cast<MainWindow*>(UiUtil::getMainWindow());
    Q_CHECK_PTR(mainWindow);

    mainWindow->getTabs()->setCurrentIndex(3);
}

void StartListener::changeFavoriteTask() {
    auto& taskDao = TaskDao::getInstance();

    const QModelIndex index = mPanel->getTable()->selectionModel()->currentIndex();
    const qint32 id = index.sibling(index.row(), ID_COLUMN).data().toInt();

    const Task savedTask = taskDao.get(id);
    const bool favorited = !savedTask.favorited;

    const Task favoriteTask = {
        .id = id,
        .idAccount = 1,
        .completed = savedTask.completed,
        .favorited = favorited,
        .timestamp = savedTask.timestamp,
        .name = savedTask.name,
        .description = savedTask.description
    };

    taskDao.update(favoriteTask);
    mPanel->updateTable();

    UiUtil::showInfoMessage(tr("Favorite tasks"),
                            tr("Task was marked as favorite!"));
}

void StartListener::changeCompleteTask() {
    auto& taskDao = TaskDao::getInstance();

    const QModelIndex index = mPanel->getTable()->selectionModel()->currentIndex();
    const qint32 id = index.sibling(index.row(), ID_COLUMN).data().toInt();

    const Task savedTask = taskDao.get(id);
    const bool completed = !savedTask.completed;

    const Task completeTask = {
        .id = id,
        .idAccount = 1,
        .completed = completed,
        .favorited = savedTask.favorited,
        .timestamp = savedTask.timestamp,
        .name = savedTask.name,
        .description = savedTask.description
    };

    taskDao.update(completeTask);
    mPanel->updateTable();

    UiUtil::showInfoMessage(tr("Complete tasks"),
                            tr("Task marked as complete!"));
}

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

#include "ui/tab/StartPanel.hpp"
#include "db/TaskDao.hpp"

#include "ui/dialog/AddTaskDialog.hpp"
#include "ui/dialog/EditTaskDialog.hpp"

#include "ui/main/MainWindow.hpp"
#include "ui/util/UiUtil.hpp"

#include <QLineEdit>
#include <QHeaderView>
#include <QLabel>

static constexpr const char* const TAG =  "[StartPanel] ";
static constexpr int ID_COLUMN = 0;
static constexpr int TIMESTAMP_COLUMN = 1;
static constexpr int COMPLETED_COLUMN = 2;
static constexpr int FAVORITED_COLUMN = 3;
static constexpr int NAME_COLUMN = 4;
static constexpr int DESCRIPTION_COLUMN = 5;

StartPanel::StartPanel(QWidget* parent)
    : QGroupBox(parent)
    , mModel(new TaskModel) {
    setObjectName("start-panel");

    mLayout = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(20);
#else
    mLayout->setContentsMargins(20, 20, 20, 20);
#endif
    mLayout->setSpacing(20);

    initNavigateRegion();
    initTableRegion();
    initControRegion();

    setLayout(mLayout);
}

StartPanel::~StartPanel() {
    delete mModel;
}

void StartPanel::initNavigateRegion() {
    mSearchEdit = new QLineEdit;
    mSearchEdit->setPlaceholderText(tr("Search tasks"));
    mSearchEdit->setObjectName("search-edit");

    mSearchButton = new QPushButton;
    mSearchButton->setObjectName("search-button");

    mSortButton = new QToolButton;
    mSortButton->setObjectName("sort-button");

    QMenu* menu = new QMenu;
    mSortAscAction = menu->addAction(QIcon(":res/image/type/sort-az.png"), tr("A-Z"));
    mSortDescAction = menu->addAction(QIcon(":res/image/type/sort-za.png"), tr("Z-A"));
    mSortDayAction = menu->addAction(QIcon(":res/image/type/sort-day.png"), tr("Today"));
    mSortCalendarAction = menu->addAction(QIcon(":res/image/type/sort-calendar.png"), tr("Calendar"));

    mSortButton->setMenu(menu);
    mSortButton->setPopupMode(QToolButton::InstantPopup);

    QObject::connect(mSearchButton, SIGNAL(clicked()),
                     this, SLOT(showSearchTask()));
    QObject::connect(mSortAscAction, SIGNAL(triggered(bool)),
                     this, SLOT(sortAscTask(bool)));
    QObject::connect(mSortDescAction, SIGNAL(triggered(bool)),
                     this, SLOT(sortDescTask(bool)));
    QObject::connect(mSortDayAction, SIGNAL(triggered(bool)),
                     this, SLOT(searchTodayTask(bool)));
    QObject::connect(mSortCalendarAction, SIGNAL(triggered(bool)),
                     this, SLOT(sortCalendarTask(bool)));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mSearchEdit);
    horizontalLayout->addWidget(mSearchButton);
    horizontalLayout->addWidget(mSortButton);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    horizontalLayout->setMargin(5);
#else
    horizontalLayout->setContentsMargins(5, 5, 5, 5);
#endif
    horizontalLayout->setSpacing(10);

    mLayout->addLayout(horizontalLayout);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(10);
#else
    mLayout->setContentsMargins(10, 10, 10, 10);
#endif
    mLayout->setSpacing(10);
}

void StartPanel::initTableRegion() {
    mTable = new QTableView;
    mTable->setObjectName("task-table");
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mTable->setAlternatingRowColors(true);

    setupModel();
    setupHeaderTable();

    mTable->setColumnHidden(0, true);

    createMenu();

    mLayout->addWidget(mTable);
}

void StartPanel::initControRegion() {
    mAddButton = new QPushButton;
    mAddButton->setObjectName("add-button");

    mEditButton = new QPushButton;
    mEditButton->setObjectName("edit-button");

    mSimilarButton = new QPushButton;
    mSimilarButton->setObjectName("similar-button");

    mRemoveButton = new QPushButton;
    mRemoveButton->setObjectName("trash-button");

    QObject::connect(mAddButton, SIGNAL(clicked()),
                     this, SLOT(showAddTask()));
    QObject::connect(this, SIGNAL(clicked()),
                     this, SLOT(showEditTask()));
    QObject::connect(mSimilarButton, SIGNAL(clicked()),
                     this, SLOT(showSimilarTask()));
    QObject::connect(mRemoveButton, SIGNAL(clicked()),
                     this, SLOT(showDeleteTask()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addStretch(1);
    horizontalLayout->addWidget(mAddButton);
    horizontalLayout->addWidget(mEditButton);
    horizontalLayout->addWidget(mSimilarButton);
    horizontalLayout->addWidget(mRemoveButton);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    horizontalLayout->setMargin(5);
#else
    horizontalLayout->setContentsMargins(5, 5, 5, 5);
#endif
    horizontalLayout->setSpacing(10);

    mLayout->addLayout(horizontalLayout);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(10);
#else
    mLayout->setContentsMargins(10, 10, 10, 10);
#endif
    mLayout->setSpacing(10);
}

void StartPanel::setupHeaderTable() {
    mTable->verticalHeader()->hide();
    mTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable->horizontalHeader()->setStretchLastSection(true);

    mTable->setColumnWidth(0, 50);
    mTable->setColumnWidth(1, 180);
    mTable->setColumnWidth(2, 70);
    mTable->setColumnWidth(3, 70);
    mTable->setColumnWidth(4, 150);
    mTable->setColumnWidth(5, 160);
}

void StartPanel::createMenu() {
    mTable->setContextMenuPolicy(Qt::CustomContextMenu);

    mMenu = new QMenu(this);
    mMenu->setObjectName("type-task-menu");

    mFavoriteAction = mMenu->addAction(QIcon(":res/image/type/favorite.png"),
                                       tr("Favorite"));
    mCompleteAction = mMenu->addAction(QIcon(":res/image/type/complete.png"),
                                       tr("Complete"));
    QObject::connect(mTable, SIGNAL(customContextMenuRequested(const QPoint&)),
                     this, SLOT(showTask(const QPoint&)));
    QObject::connect(mFavoriteAction, SIGNAL(triggered()),
                     this, SLOT(changeFavoriteTask()));
    QObject::connect(mCompleteAction, SIGNAL(triggered()),
                     this, SLOT(changeCompleteTask()));
}

void StartPanel::setupModel() {
    QList<Task> list = TaskDao::getInstance().getAll();

    mModel->setTasks(std::move(list));
    mTable->setModel(mModel);
}

void StartPanel::updateTable() {
    QList<Task> tasks = TaskDao::getInstance().getAll();

    mModel->setTasks(std::move(tasks));
}

void StartPanel::updateTable(QList<Task>&& tasks) {
    mModel->setTasks(std::move(tasks));
}

void StartPanel::showTask(const QPoint& position) {
    const QPoint newPosition = mTable->mapToGlobal(position);

    mMenu->exec(newPosition);
}

void StartPanel::showAddTask() {
    AddTaskDialog dialog(tr("Add task"));
    //dialog.setParent(this);

    if (dialog.exec() == QDialog::Accepted) {
        qInfo() << TAG << "Dialog was closed!" << '\n';
    }
}

void StartPanel::showEditTask() {
    const QString format = "dd-MM-yyyy_hh:mm:ss";
    const QModelIndex index = mTable->selectionModel()->currentIndex();

    if (index.isValid()) {
        EditTaskDialog dialog(tr("Edit task"));
        //dialog.setParent(this);

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
            qInfo() << TAG << "Dialog was closed!" << '\n';
        }
    } else {
        UiUtil::showInfoMessage(tr("Edit task"),
                                tr("Line was not choosed!"));
    }
}

void StartPanel::showDeleteTask() {
   const QModelIndex index = mTable->selectionModel()->currentIndex();

    if (index.isValid()) {
        const qint32 number  = index.sibling(index.row(), ID_COLUMN).data().toInt();
        TaskDao::getInstance().remove(number);

        updateTable();

        UiUtil::showInfoMessage(tr("Delete task"),
                                tr("Task was deleted success!"));
    } else {
        UiUtil::showErrorMessage(tr("Delete task"),
                                 tr("Line was not choosed!"));
    }
}

void StartPanel::showSimilarTask() {
    const QModelIndex index = mTable->selectionModel()->currentIndex();

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
        updateTable();
    } else {
        UiUtil::showErrorMessage(tr("Duplicate task"),
                                 tr("Line was not choosed!"));
    }
}

void StartPanel::showSearchTask() {
    const QString text = mSearchEdit->text();

    if (!text.isEmpty()) {
        auto& taskDao = TaskDao::getInstance();

        updateTable(taskDao.search(text));
    } else {
        UiUtil::showErrorMessage(tr("Search tasks"),
                                 tr("Line was not input!"));
        updateTable();
    }
}

void StartPanel::sortAscTask(bool asc) {
    Q_UNUSED(asc);

    auto& taskDao = TaskDao::getInstance();
    QList<Task> tasks = taskDao.sort(true);

    if (!tasks.empty()) {
        updateTable(std::move(tasks));
    } else {
        UiUtil::showErrorMessage(tr("Sort tasks"),
                                 tr("List tasks are empty!"));
    }
}

void StartPanel::sortDescTask(bool asc) {
    Q_UNUSED(asc);

    auto& taskDao = TaskDao::getInstance();
    QList<Task> tasks = taskDao.sort(false);

    if (!tasks.empty()) {
        updateTable(std::move(tasks));
    } else {
        UiUtil::showErrorMessage(tr("Sort tasks"),
                                 tr("List tasks are empty!"));
    }
}

void StartPanel::searchTodayTask(bool checked) {
    Q_UNUSED(checked);

    auto& taskDao = TaskDao::getInstance();
    QList<Task> tasks = taskDao.search(QDate::currentDate());

    if (!tasks.empty()) {
        updateTable(std::move(tasks));
    } else {
        UiUtil::showErrorMessage(tr("Search tasks"),
                                 tr("List tasks are empty!"));
    }
}

void StartPanel::sortCalendarTask(bool checked) {
    Q_UNUSED(checked);

    auto* mainWindow = static_cast<MainWindow*>(UiUtil::getMainWindow());
    Q_CHECK_PTR(mainWindow);

    mainWindow->getTabs()->setCurrentIndex(3);
}

void StartPanel::changeFavoriteTask() {
    auto& taskDao = TaskDao::getInstance();

    const QModelIndex index = mTable->selectionModel()->currentIndex();
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
    updateTable();

    UiUtil::showInfoMessage(tr("Favorite tasks"),
                            tr("Task was marked as favorite!"));
}

void StartPanel::changeCompleteTask() {
    auto& taskDao = TaskDao::getInstance();

    const QModelIndex index = mTable->selectionModel()->currentIndex();
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
    updateTable();

    UiUtil::showInfoMessage(tr("Complete tasks"),
                            tr("Task marked as complete!"));
}

TaskModel* StartPanel::getModel() const {
    return mModel;
}


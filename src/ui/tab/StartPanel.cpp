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

#include <QLineEdit>
#include <QHeaderView>
#include <QLabel>

static constexpr const char* const TAG =  "[StartPanel] ";

StartPanel::StartPanel(QWidget* parent)
    : QGroupBox(parent)
    , mModel(new TaskModel)
    , mListener(new StartEvent(this)) {
    setObjectName("start-panel");

    mLayout = new QVBoxLayout;
    mLayout->setMargin(20);
    mLayout->setSpacing(20);

    initNavigateRegion();
    initTableRegion();
    initControRegion();

    setLayout(mLayout);
}

StartPanel::~StartPanel() {
    delete mModel;
    delete mListener;
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
                     mListener,     SLOT(showSearchTask()));
    QObject::connect(mSortAscAction, SIGNAL(triggered(bool)),
                     mListener,      SLOT(sortAscTask(bool)));
    QObject::connect(mSortDescAction, SIGNAL(triggered(bool)),
                     mListener,       SLOT(sortDescTask(bool)));
    QObject::connect(mSortDayAction,  SIGNAL(triggered(bool)),
                     mListener,       SLOT(searchTodayTask(bool)));
    QObject::connect(mSortCalendarAction, SIGNAL(triggered(bool)),
                     mListener,           SLOT(sortCalendarTask(bool)));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mSearchEdit);
    horizontalLayout->addWidget(mSearchButton);
    horizontalLayout->addWidget(mSortButton);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(10);

    mLayout->addLayout(horizontalLayout);
    mLayout->setMargin(10);
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
                     mListener,  SLOT(showAddTask()));
    QObject::connect(mEditButton, SIGNAL(clicked()),
                     mListener,   SLOT(showEditTask()));
    QObject::connect(mSimilarButton, SIGNAL(clicked()),
                     mListener,      SLOT(showSimilarTask()));
    QObject::connect(mRemoveButton, SIGNAL(clicked()),
                     mListener,     SLOT(showDeleteTask()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addStretch(1);
    horizontalLayout->addWidget(mAddButton);
    horizontalLayout->addWidget(mEditButton);
    horizontalLayout->addWidget(mSimilarButton);
    horizontalLayout->addWidget(mRemoveButton);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(10);

    mLayout->addLayout(horizontalLayout);
    mLayout->setMargin(10);
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
                     mListener, SLOT(showTask(const QPoint&)));
    QObject::connect(mFavoriteAction, SIGNAL(triggered()),
                     mListener,       SLOT(changeFavoriteTask()));
    QObject::connect(mCompleteAction, SIGNAL(triggered()),
                     mListener,       SLOT(changeCompleteTask()));
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

QLineEdit* StartPanel::getSearcheEdit() const {
    return mSearchEdit;
}

QTableView* StartPanel::getTable() const {
    return mTable;
}

QMenu* StartPanel::getMenu() const {
    return mMenu;
}

TaskModel* StartPanel::getModel() const {
    return mModel;
}

QAction* StartPanel::getSortAscAction() const {
    return mSortAscAction;
}

QAction* StartPanel::getSortDescAction() const {
    return mSortDescAction;
}

QAction* StartPanel::getSortDayAction() const {
    return mSortDayAction;
}

QAction* StartPanel::getSortCalendarAction() const {
    return mSortCalendarAction;
}

QAction* StartPanel::getFavoriteAction() const {
    return mFavoriteAction;
}

QAction* StartPanel::getCompleteAction() const {
    return mCompleteAction;
}

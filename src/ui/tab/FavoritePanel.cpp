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

#include "ui/tab/FavoritePanel.hpp"
#include "db/TaskDao.hpp"

#include <QHeaderView>
#include <QMenu>
#include <QLabel>

static constexpr const char* const TAG =  "[FavoritePanel] ";

FavoritePanel::FavoritePanel(QWidget *parent)
    : QGroupBox(parent)
    , mModel(new TaskModel)
    , mListener(new FavoriteEvent(this)) {
    setObjectName("favorite-panel");

    mLayout = new QVBoxLayout;
    mLayout->setMargin(20);
    mLayout->setSpacing(20);

    initTable();
    initButtons();

    setLayout(mLayout);
}

FavoritePanel::~FavoritePanel() {
    delete mModel;
    delete mListener;
}

void FavoritePanel::initTable() {
    mTable = new QTableView;
    mTable->setObjectName("favorite-task-table");
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mTable->setAlternatingRowColors(true);

    setupModel();
    setupHeaderTable();

    mTable->setColumnHidden(0, true);

    mLayout->addWidget(mTable);
}

void FavoritePanel::initButtons() {
    mUpdateButton = new QPushButton;
    mUpdateButton->setObjectName("update-button");

    QObject::connect(mUpdateButton, SIGNAL(clicked()),
                     mListener,     SLOT(showUpdateTask()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addStretch(1);
    horizontalLayout->addWidget(mUpdateButton);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(10);

    mLayout->addLayout(horizontalLayout);
    mLayout->setMargin(10);
    mLayout->setSpacing(10);
}

void FavoritePanel::setupHeaderTable() {
    mTable->verticalHeader()->hide();
    mTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable->horizontalHeader()->setStretchLastSection(true);

    mTable->setColumnWidth(0, 60);
    mTable->setColumnWidth(1, 200);
    mTable->setColumnWidth(2, 90);
    mTable->setColumnWidth(3, 90);
    mTable->setColumnWidth(4, 200);
}

void FavoritePanel::setupModel() {
    QList<Task> list = TaskDao::getInstance().getAllFavorite();

    mModel->setTasks(std::move(list));
    mTable->setModel(mModel);
}

//FIXME: move to Listener
void FavoritePanel::updateTable() {
    QList<Task> list = TaskDao::getInstance().getAllFavorite();

    mModel->setTasks(std::move(list));
}

QTableView* FavoritePanel::getTable() const {
    return mTable;
}

FavoriteEvent* FavoritePanel::getEvent() const {
    return mListener;
}

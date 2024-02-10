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

#include "ui/tab/CalendarPanel.hpp"
#include "db/TaskDao.hpp"

#include <QToolButton>
#include <QLabel>

static constexpr const char* const TAG =  "[CalendarPanel] ";

CalendarPanel::CalendarPanel(QWidget* parent)
    : QGroupBox(parent) {
    setObjectName("calendar-panel");

    mLayout = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(20);
#else
    mLayout->setContentsMargins(20, 20, 20, 20);
#endif
    mLayout->setSpacing(20);

    initCalendar();
    initButtons();

    setLayout(mLayout);
}

CalendarPanel::~CalendarPanel() {
}

void CalendarPanel::initCalendar() {
    const QDate startDate(1900, 1, 1);
    const QDate endDate(3000, 1, 1);

    mCalendar = new TaskCalendar;
    mCalendar->setMinimumDate(startDate);
    mCalendar->setMaximumDate(endDate);
    mCalendar->setGridVisible(true);

    QToolButton* previousMonth = mCalendar->findChild<QToolButton*>("qt_calendar_prevmonth");
    QToolButton* nextMonth = mCalendar->findChild<QToolButton*>("qt_calendar_nextmonth");

    previousMonth->setIcon(QIcon(":res/image/type/back.png"));
    nextMonth->setIcon(QIcon(":res/image/type/forward.png"));

    mCalendar->setListTask(TaskDao::getInstance().getAll());

    mLayout->addWidget(mCalendar);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(10);
#else
    mLayout->setContentsMargins(10, 10, 10, 10);
#endif
    mLayout->setSpacing(10);
}

void CalendarPanel::initButtons() {
    this->mUpdateButton = new QPushButton;
    this->mUpdateButton->setObjectName("update-button");

    QObject::connect(mUpdateButton, SIGNAL(clicked()),
                     this, SLOT(showUpdateCalendar()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addStretch(1);
    horizontalLayout->addWidget(mUpdateButton);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    horizontalLayout->setMargin(5);
#else
    mLayout->setContentsMargins(5, 5, 5, 5);
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

void CalendarPanel::showUpdateCalendar() {
    const QList<Task> tasks = TaskDao::getInstance().getAll();

    mCalendar->setListTask(tasks);
}

void CalendarPanel::updateCalendar(int index) {
    Q_UNUSED(index);

    const QList<Task> tasks = TaskDao::getInstance().getAll();

    mCalendar->setListTask(tasks);
}

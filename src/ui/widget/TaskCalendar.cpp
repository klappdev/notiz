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

#include "ui/widget/TaskCalendar.hpp"
#include "setting/AppSettings.hpp"

static constexpr const char* const TAG =  "[CalendarPanel] ";

TaskCalendar::TaskCalendar(QWidget* parent)
    : QCalendarWidget(parent) {
    retranslateUi();
}

TaskCalendar::~TaskCalendar() {}

void TaskCalendar::paintCell(QPainter* painter, const QRect& rect, QDate date) const {
    QCalendarWidget::paintCell(painter, rect, date);
    mCounter = 0;

    for (const Task& task : mTasks) {
        if (date.toString("dd-MM-yyyy") == task.timestamp.split("_")[0]) {
            mTitle = task.name;
            mCounter++;

            break;
        }
    }

    if (mCounter != 0) {
        if (mTitle.length() > 10) {
            mTitle = mTitle.left(10) + " ... ";
        }

        painter->drawText(rect.topLeft() + QPoint(15, 15),  mTitle);
    }
}

void TaskCalendar::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }

    QCalendarWidget::changeEvent(event);
}

void TaskCalendar::retranslateUi() {
    auto& settings = AppSettings::getInstance();
    const QString& language = settings.getLanguage();

    setLocale(QLocale(language));

    qCritical() << TAG << "Change language: " << language << '\n';
}

void TaskCalendar::setListTask(const QList<Task>& tasks) {
    this->mTasks = tasks;
}

void TaskCalendar::setListTask(QList<Task>&& tasks) {
    this->mTasks = std::move(tasks);
}

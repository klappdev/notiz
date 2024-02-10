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

#include "ui/dialog/CalendarDialog.hpp"

#include <QToolButton>

CalendarDialog::CalendarDialog(const QString& title)
    : QDialog() {
    setWindowTitle(title);
    setObjectName("calendar-dialog");

    setWindowIcon(QIcon(":res/image/dialog/add-task.png"));
    setWindowModality(Qt::WindowModal);

    initUI();

    setLayout(mLayout);

    setFixedSize(350, 250);
}

CalendarDialog::~CalendarDialog() {
}

void CalendarDialog::initUI() {
    mCalendar = new QCalendarWidget;
    mCalendar->setMinimumDate(QDate(1900, 1, 1));
    mCalendar->setMaximumDate(QDate(2050, 1, 1));
    mCalendar->setGridVisible(true);

    auto* previousMonth = mCalendar->findChild<QToolButton*>("qt_calendar_prevmonth");
    auto* nextMonth = mCalendar->findChild<QToolButton*>("qt_calendar_nextmonth");

    previousMonth->setIcon(QIcon(":res/image/type/back.png"));
    nextMonth->setIcon(QIcon(":res/image/type/forward.png"));

    QObject::connect(mCalendar, SIGNAL(clicked(QDate)),
                     this, SLOT(selectDate()));

    mLayout = new QVBoxLayout;
    mLayout->addWidget(mCalendar);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(10);
#else
    mLayout->setContentsMargins(10, 10, 10, 10);
#endif
    mLayout->setSpacing(10);
}

void CalendarDialog::selectDate() {
    const QString format = "dd-MM-yyyy_hh:mm:ss";

    const QString selectDate = mCalendar->selectedDate().toString(format);
    [[maybe_unused]] const QDateTime selectDateTime = QDateTime::fromString(selectDate, format);

    /*
    mTimestampEdit->setDateTime(selectDateTime);
    */
    close();
}

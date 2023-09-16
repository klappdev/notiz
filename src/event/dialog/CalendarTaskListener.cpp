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

#include "event/dialog/CalendarTaskListener.hpp"

static constexpr const char* const TAG = "[CalendarTaskListener] ";

CalendarTaskListener::CalendarTaskListener(CalendarDialog* dialog)
    : mDialog(dialog) {
    Q_ASSERT(mDialog);
}

CalendarTaskListener::~CalendarTaskListener() {}

void CalendarTaskListener::selectDate() {
    const QString format = "dd-MM-yyyy_hh:mm:ss";

    const QString selectDate = mDialog->getCalendar()->selectedDate().toString(format);
    [[maybe_unused]] const QDateTime selectDateTime = QDateTime::fromString(selectDate, format);

    /*
    mDialog->getTimestampEdit()->setDateTime(selectDateTime);
    */
    mDialog->close();
}

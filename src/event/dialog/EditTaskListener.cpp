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

#include "event/dialog/EditTaskListener.hpp"
#include "db/TaskDao.hpp"

#include "ui/util/UiUtil.hpp"
#include "ui/dialog/CalendarDialog.hpp"

#include <QLineEdit>

static constexpr const char* const TAG = "[EditTaskListener] ";

EditTaskListener::EditTaskListener(EditTaskDialog* dialog) :
    mDialog(dialog) {
}

EditTaskListener::~EditTaskListener() {}

void EditTaskListener::confirm() {
    Q_ASSERT(mDialog != nullptr);

    const qint32 number  = mDialog->getNumberRow();
    const bool completed = mDialog->getCompleteCheckBox()->isChecked();
    const bool favorited = mDialog->getFavoriteCheckBox()->isChecked();

    const QString timestamp = mDialog->getTimestampEdit()->text();
    const QString name = mDialog->getNameEdit()->text();
    const QString description = mDialog->getDescriptionEdit()->toPlainText();

    //FIXME: add regex check timestamp
    if (!timestamp.isEmpty() && !description.isEmpty()) {
        const Task task = {
            .id = number,
            .idAccount = 1,
            .completed = completed,
            .favorited = favorited,
            .timestamp = timestamp,
            .name = name,
            .description = description
        };

        const bool success = TaskDao::getInstance().update(task);

        UiUtil::showInfoMessage(tr("Edit task"),
                                success ? tr("Task was edited success!")
                                        : tr("Can't edit work error"));

        cancel();
    } else {
        UiUtil::showErrorMessage(tr("Incorrect data was input"),
                                 tr("Input data are not correct!"));
    }
}

void EditTaskListener::cancel() {
    mDialog->reject();
    mDialog->getParent()->updateTable();

    qInfo() << "close edit task dialog" << endl;
}

void EditTaskListener::selectDate() {
    CalendarDialog dialog(tr("Calendar"));
    dialog.setParent(mDialog);

    if (dialog.exec() == QDialog::Accepted) {
        qInfo() << TAG << "Dialog was closed!" << endl;
    }
}


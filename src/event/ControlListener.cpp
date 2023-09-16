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

#include "event/ControlListener.hpp"
#include "setting/AppSettings.hpp"

#include "ui/util/UiUtil.hpp"
#include "ui/dialog/LoginDialog.hpp"

#include <QApplication>
#include <QDateTime>

static constexpr const char* const TAG = "[ControlListener] ";

ControlListener::ControlListener(QWidget* parent)
    : mMainWindow((MainWindow*) parent) {
}

ControlListener::~ControlListener() {}

void ControlListener::notifyTasks() {
    const QString format = "dd-MM-yyyy_hh:mm";
    const QList<Task>& tasks = mMainWindow->getStartPanel()->getModel()->getTasks();

    for (const Task& task : tasks) {
        /* without seconds */
        const QString currentDateTime = QDateTime::currentDateTime().toString(format);
        const QString taskDateTime = task.timestamp.left(task.timestamp.lastIndexOf(':'));

        if (currentDateTime == taskDateTime) {
            UiUtil::showInfoMessage(tr("Notify task"),
                                    tr("Start notify task: %1\n").arg(task.name));
        }
    }
}

void ControlListener::exit() {
    auto* loginDialog = static_cast<LoginDialog*>(UiUtil::getLoginWindow());
    Q_CHECK_PTR(loginDialog);
    AppSettings::getInstance().store();

    loginDialog->reject();
    qApp->quit();
}



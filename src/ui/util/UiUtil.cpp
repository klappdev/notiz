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

#include "ui/util/UiUtil.hpp"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QIcon>

namespace UiUtil {

    QWidget* getMainWindow() {
        for (QWidget* current : qApp->topLevelWidgets()) {
            if (current->objectName() == "main-window") {
                return current;
            }
        }

        return nullptr;
    }

    QWidget* getLoginWindow() {
        for (QWidget* current : qApp->topLevelWidgets()) {

            if (current->objectName() == "login-window") {
                return current;
            }
        }

        return nullptr;
    }

    void printObjectsTree() {
        const QObjectList list = getMainWindow()->findChildren<QObject*>();

        for (QObject* child : list) {
            qDebug() << child->objectName();
        }
    }

    void showInfoMessage(const QString& title, const QString& text) {
        QMessageBox messageBox;
        messageBox.setText(title);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setWindowIcon(QIcon(":res/image/task_icon.png"));
        messageBox.setInformativeText(text);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setFixedSize(300, 200);

        messageBox.exec();
    }

    void showErrorMessage(const QString& title, const QString& text) {
        QMessageBox messageBox;
        messageBox.setText(title);
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setWindowIcon(QIcon(":res/image/task_icon.png"));
        messageBox.setInformativeText(text);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setFixedSize(300, 200);

        messageBox.exec();
    }
}

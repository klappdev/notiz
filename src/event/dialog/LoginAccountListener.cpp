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

#include "event/dialog/LoginAccountListener.hpp"
#include "db/AccountDao.hpp"

#include "ui/util/UiUtil.hpp"

static constexpr const char* const TAG = "[LoginAccountListener] ";

LoginAccountListener::LoginAccountListener(LoginDialog* parent)
    : mLoginDialog(parent) {
    Q_ASSERT(mLoginDialog);
}

LoginAccountListener::~LoginAccountListener() {}

void LoginAccountListener::successLoad() {
    const QString login = mLoginDialog->getUserName();
    const QString password = mLoginDialog->getPassword();

    mMainWindow = static_cast<MainWindow*>(UiUtil::getMainWindow());
    Q_CHECK_PTR(mMainWindow);

    if (AccountDao::getInstance().checkIfExists(login, password)) {
        mLoginDialog->hide();

        mMainWindow->show();
    } else {
        mLoginDialog->setUserName("");
        mLoginDialog->setPassword("");

        UiUtil::showErrorMessage(tr("Enter user in application"),
                                 tr("Login and password was input incorrect!"));
    }
}

void LoginAccountListener::changeMode(bool changed) {
    if (changed) {
        mLoginDialog->getPasswordEdit()->setEchoMode(QLineEdit::Password);
    } else {
        mLoginDialog->getPasswordEdit()->setEchoMode(QLineEdit::Normal);
    }
}


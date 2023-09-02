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

#include "ui/dialog/LoginDialog.hpp"

#include <QApplication>
#include <QPushButton>
#include <QStringList>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
    , mListener(new LoginEvent(this)) {
    setWindowTitle(tr("Login account"));
    setObjectName("login-window");

    setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint);

    setWindowIcon(QIcon(":res/image/login/login-icon.png"));

    setupUI();

    resize(300, 300);
}

LoginDialog::~LoginDialog() {
    delete mListener;
}

void LoginDialog::setupUI() {
    mMainLayout = new QVBoxLayout;

    setupTopRegion();
    setupBottomRegion();

    mMainLayout->addWidget(mAvatarLabel);
    mMainLayout->addWidget(mUserNameLabel);
    mMainLayout->addWidget(mUserNameEdit);

    mMainLayout->addWidget(mPasswordLabel);
    mMainLayout->addWidget(mPasswordEdit);

    /* mainLayout->addWidget(changeModeCheckBox); */
    mMainLayout->addWidget(mButtons);

    mMainLayout->setAlignment(mAvatarLabel, Qt::AlignHCenter);
    mMainLayout->setAlignment(mButtons, Qt::AlignHCenter);
    mMainLayout->setMargin(30);
    mMainLayout->setSpacing(20);

    setLayout(mMainLayout);
}

void LoginDialog::setupTopRegion() {
    mAvatarLabel = new QLabel;
    mAvatarLabel->setPixmap(QPixmap(":res/image/login/default-login.png"));

    mUserNameEdit = new QLineEdit("user1");
    mUserNameEdit->setPlaceholderText("login");

    mPasswordEdit = new QLineEdit("user1");
    mPasswordEdit->setPlaceholderText("password");
    mPasswordEdit->setEchoMode(QLineEdit::Password);

    mUserNameLabel = new QLabel;
    mUserNameLabel->setText(tr("Name account"));
    mUserNameLabel->setBuddy(mUserNameEdit);

    mPasswordLabel = new QLabel;
    mPasswordLabel->setText(tr("Password"));
    mPasswordLabel->setBuddy(mPasswordEdit);
}

void LoginDialog::setupBottomRegion() {
    mChangeModeCheckBox = new QCheckBox(tr("Hide text"));
    mChangeModeCheckBox->setChecked(true);

    mButtons = new QDialogButtonBox;
    mButtons->addButton(QDialogButtonBox::Ok);
    mButtons->addButton(QDialogButtonBox::Cancel);

    mButtons->button(QDialogButtonBox::Ok)->setText(tr(" Enter "));
    mButtons->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    QObject::connect(mButtons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
                     qApp,                                       SLOT(quit()));

    QObject::connect(mButtons->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
                     mListener,                              SLOT(successLoad()));

    QObject::connect(mChangeModeCheckBox, SIGNAL(toggled(bool)),
                     mListener,           SLOT(changeMode(bool)));
}

void LoginDialog::setPassword(const QString &password){
    mPasswordEdit->setText(password);
}

void LoginDialog::setUserName(const QString& name){
    mUserNameEdit->setText(name);
}

QString LoginDialog::getUserName() const {
    return mUserNameEdit->text();
}

QString LoginDialog::getPassword() const {
    return mPasswordEdit->text();
}

QLineEdit* LoginDialog::getPasswordEdit() {
    return mPasswordEdit;
}

LoginEvent* LoginDialog::getEvent() const {
    return mListener;
}

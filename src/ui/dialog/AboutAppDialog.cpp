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

#include "ui/dialog/AboutAppDialog.hpp"

#include <QLabel>

AboutAppDialog::AboutAppDialog() : QDialog() {
    setWindowTitle(tr("About program"));
    setObjectName("about-app-dialog");

    setWindowIcon(QIcon(":res/image/task_icon.png"));
    setWindowModality(Qt::WindowModal);

    initUI();
    setLayout(mLayout);

    setFixedSize(400, 400);
}

AboutAppDialog::~AboutAppDialog() {}

void AboutAppDialog::initUI() {
    mInfoArea = new QTextEdit;
    mInfoArea->setObjectName("info-edit");
    mInfoArea->setReadOnly(true);
    mInfoArea->setText(tr("Program use for manage notices user!"));

    mCancelButton = new QPushButton;
    mCancelButton->setObjectName("cancel-button");

    QObject::connect(mCancelButton, SIGNAL(clicked()),
                     this,          SLOT(reject()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addStretch(1);
    horizontalLayout->addWidget(mCancelButton);

    mLayout = new QVBoxLayout;
    mLayout->addWidget(mInfoArea);
    mLayout->addLayout(horizontalLayout);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mLayout->setMargin(10);
#else
    mLayout->setContentsMargins(10, 10, 10, 10);
#endif
    mLayout->setSpacing(10);
}

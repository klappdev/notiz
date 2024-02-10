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

#include "ui/dialog/AddTaskDialog.hpp"
#include "db/TaskDao.hpp"

#include "ui/util/UiUtil.hpp"
#include "ui/dialog/CalendarDialog.hpp"

static constexpr const char* const TAG = "[AddTaskDialog] ";

AddTaskDialog::AddTaskDialog(const QString& title)
    : mPanel(nullptr) {
    setWindowTitle(title);
    setObjectName("add-task-dialog");

    setWindowIcon(QIcon("image/dialog/add-task.png"));
    setWindowModality(Qt::WindowModal);

    initUI();

    setLayout(mVerticalBox);

    setFixedSize(300, 450);
}

AddTaskDialog::~AddTaskDialog() {
}

void AddTaskDialog::initUI() {
    initFlagRegion();
    initTimestampRegion();

    initNameRegion();
    initDescriptionRegion();

    initControlRegion();

    mVerticalBox = new QVBoxLayout;
    mVerticalBox->addLayout(mCompleteLayout);
    mVerticalBox->addLayout(mFavoriteLayout);

    mVerticalBox->addLayout(mTimestampLayout);
    mVerticalBox->addLayout(mNameLayout);
    mVerticalBox->addLayout(mDescriptionLayout);
    mVerticalBox->addStretch(1);

    mVerticalBox->addLayout(mControlLayout);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mVerticalBox->setMargin(10);
#else
    mVerticalBox->setContentsMargins(10, 10, 10, 10);
#endif
    mVerticalBox->setSpacing(10);
}

void AddTaskDialog::initFlagRegion() {
    mCompleteLabel = new QLabel(tr("Complete: "));
    mFavoriteLabel = new QLabel(tr("Favorite: "));

    mCompleteCheckBox = new QCheckBox;
    mFavoriteCheckBox = new QCheckBox;

    mCompleteCheckBox->setObjectName("complete-checkbox");
    mFavoriteCheckBox->setObjectName("favorite-checkbox");

    mCompleteLabel->setVisible(false);
    mCompleteCheckBox->setVisible(false);

    mCompleteLayout = new QHBoxLayout;
    mCompleteLayout->addWidget(mCompleteLabel);
    mCompleteLayout->addWidget(mCompleteCheckBox);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mCompleteLayout->setMargin(5);
#else
    mCompleteLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mCompleteLayout->setSpacing(5);

    mFavoriteLayout = new QHBoxLayout;
    mFavoriteLayout->addWidget(mFavoriteLabel);
    mFavoriteLayout->addWidget(mFavoriteCheckBox);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mFavoriteLayout->setMargin(5);
#else
    mFavoriteLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mFavoriteLayout->setSpacing(5);
}

void AddTaskDialog::initTimestampRegion() {
    mTimestampLabel = new QLabel(tr("Time:"));

    mTimestampEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    mTimestampEdit->setDisplayFormat("dd-MM-yyyy_hh:mm:ss");
    mTimestampEdit->setObjectName("timestamp-edit");

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mTimestampEdit);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    horizontalLayout->setMargin(5);
#else
    horizontalLayout->setContentsMargins(5, 5, 5, 5);
#endif
    horizontalLayout->setSpacing(5);

    mTimestampLayout = new QVBoxLayout;
    mTimestampLayout->addWidget(mTimestampLabel);
    mTimestampLayout->addLayout(horizontalLayout);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mTimestampLayout->setMargin(5);
#else
    mTimestampLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mTimestampLayout->setSpacing(5);
}

void AddTaskDialog::initNameRegion() {
    mNameLabel = new QLabel(tr("Name:"));

    mNameEdit = new QLineEdit;
    mNameEdit->setObjectName("description-edit");

    mNameLayout = new QVBoxLayout;
    mNameLayout->addWidget(mNameLabel);
    mNameLayout->addWidget(mNameEdit);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mNameLayout->setMargin(5);
#else
    mNameLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mNameLayout->setSpacing(5);
}

void AddTaskDialog::initDescriptionRegion() {
    mDescriptionLabel = new QLabel(tr("Description:"));

    mDescriptionEdit = new QTextEdit;
    mDescriptionEdit->setObjectName("description-edit");

    mDescriptionLayout = new QVBoxLayout;
    mDescriptionLayout->addWidget(mDescriptionLabel);
    mDescriptionLayout->addWidget(mDescriptionEdit);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mDescriptionLayout->setMargin(5);
#else
    mDescriptionLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mDescriptionLayout->setSpacing(5);
}

void AddTaskDialog::initControlRegion() {
    mConfirmButton = new QPushButton;
    mConfirmButton->setObjectName("confirm-button");

    mCancelButton = new QPushButton;
    mCancelButton->setObjectName("cancel-button");

    QObject::connect(mConfirmButton, SIGNAL(clicked()),
                     this, SLOT(confirm()));
    QObject::connect(mCancelButton,  SIGNAL(clicked()),
                     this, SLOT(cancel()));

    mControlLayout = new QHBoxLayout;
    mControlLayout->addWidget(mConfirmButton);
    mControlLayout->addWidget(mCancelButton);
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mControlLayout->setMargin(10);
#else
    mControlLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mControlLayout->setSpacing(5);
}

void AddTaskDialog::confirm() {
    const bool completed = mCompleteCheckBox->isChecked();
    const bool favorited = mFavoriteCheckBox->isChecked();

    const QString timestamp = mTimestampEdit->text();
    const QString name = mNameEdit->text();
    const QString description = mDescriptionEdit->toPlainText();

    //FIXME: add regex check timestamp
    if (!timestamp.isEmpty() && !name.isEmpty() && !description.isEmpty()) {
        const Task task = {
            .id = 0,
            .idAccount = 1,
            .completed = completed,
            .favorited = favorited,
            .timestamp = timestamp,
            .name = name,
            .description = description
        };

        const bool success = TaskDao::getInstance().add(task);

        UiUtil::showInfoMessage(tr("Add task"),
                                success ? tr("Task was added success!")
                                        : tr("Can't add task error"));

        mDescriptionEdit->setText("");

        mCompleteCheckBox->setChecked(false);
        mFavoriteCheckBox->setChecked(false);

        cancel();
    } else {
        UiUtil::showErrorMessage(tr("Incorrect data was input"),
                                 tr("Input data are not correct!"));
    }
}

void AddTaskDialog::cancel() {
    //reject();
    close();
    mPanel->updateTable();

    qInfo() << TAG << "Close add task dialog" << '\n';
}

void AddTaskDialog::selectDate() {
    CalendarDialog dialog(tr("Calendar"));
    dialog.setParent(this);

    if (dialog.exec() == QDialog::Accepted) {
        qInfo() << TAG << "Dialog was closed!" << '\n';
    }
}

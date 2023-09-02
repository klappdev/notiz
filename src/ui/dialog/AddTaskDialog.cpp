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

AddTaskDialog::AddTaskDialog(const QString & title)
    : mListener(new AddTaskEvent(this))
    , mPanel(nullptr) {
    setWindowTitle(title);
    setObjectName("add-task-dialog");

    setWindowIcon(QIcon("image/dialog/add-task.png"));
    setWindowModality(Qt::WindowModal);

    initUI();

    setLayout(mVerticalBox);

    setFixedSize(300, 450);
}

AddTaskDialog::~AddTaskDialog() {
    delete mListener;
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
    mVerticalBox->setMargin(10);
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
    mCompleteLayout->setMargin(5);
    mCompleteLayout->setSpacing(5);

    mFavoriteLayout = new QHBoxLayout;
    mFavoriteLayout->addWidget(mFavoriteLabel);
    mFavoriteLayout->addWidget(mFavoriteCheckBox);
    mFavoriteLayout->setMargin(5);
    mFavoriteLayout->setSpacing(5);
}

void AddTaskDialog::initTimestampRegion() {
    mTimestampLabel = new QLabel(tr("Time:"));

    mTimestampEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    mTimestampEdit->setDisplayFormat("dd-MM-yyyy_hh:mm:ss");
    mTimestampEdit->setObjectName("timestamp-edit");

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mTimestampEdit);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    mTimestampLayout = new QVBoxLayout;
    mTimestampLayout->addWidget(mTimestampLabel);
    mTimestampLayout->addLayout(horizontalLayout);
    mTimestampLayout->setMargin(5);
    mTimestampLayout->setSpacing(5);
}

void AddTaskDialog::initNameRegion() {
    mNameLabel = new QLabel(tr("Name:"));

    mNameEdit = new QLineEdit;
    mNameEdit->setObjectName("description-edit");

    mNameLayout = new QVBoxLayout;
    mNameLayout->addWidget(mNameLabel);
    mNameLayout->addWidget(mNameEdit);
    mNameLayout->setMargin(5);
    mNameLayout->setSpacing(5);
}

void AddTaskDialog::initDescriptionRegion() {
    mDescriptionLabel = new QLabel(tr("Description:"));

    mDescriptionEdit = new QTextEdit;
    mDescriptionEdit->setObjectName("description-edit");

    mDescriptionLayout = new QVBoxLayout;
    mDescriptionLayout->addWidget(mDescriptionLabel);
    mDescriptionLayout->addWidget(mDescriptionEdit);
    mDescriptionLayout->setMargin(5);
    mDescriptionLayout->setSpacing(5);
}

void AddTaskDialog::initControlRegion() {
    mConfirmButton = new QPushButton;
    mConfirmButton->setObjectName("confirm-button");

    mCancelButton = new QPushButton;
    mCancelButton->setObjectName("cancel-button");

    QObject::connect(mConfirmButton, SIGNAL(clicked()),
                     mListener,      SLOT(confirm()));
    QObject::connect(mCancelButton,  SIGNAL(clicked()),
                     mListener,      SLOT(cancel()));

    mControlLayout = new QHBoxLayout;
    mControlLayout->addWidget(mConfirmButton);
    mControlLayout->addWidget(mCancelButton);
    mControlLayout->setMargin(10);
    mControlLayout->setSpacing(5);
}

QCheckBox* AddTaskDialog::getCompleteCheckBox() const {
    return mCompleteCheckBox;
}

QCheckBox* AddTaskDialog::getFavoriteCheckBox() const {
    return mFavoriteCheckBox;
}

QDateTimeEdit* AddTaskDialog::getTimestampEdit() const {
    return mTimestampEdit;
}

QLineEdit* AddTaskDialog::getNameEdit() const {
    return mNameEdit;
}

QTextEdit* AddTaskDialog::getDescriptionEdit() const {
    return mDescriptionEdit;
}

QPushButton* AddTaskDialog::getConfirmButton() const {
    return mConfirmButton;
}

QPushButton* AddTaskDialog::getCancelButton() const {
    return mCancelButton;
}

StartPanel* AddTaskDialog::getParent() const {
    return mPanel;
}

void AddTaskDialog::setParent(StartPanel* parent) {
    mPanel = parent;
}

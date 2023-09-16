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

#pragma once

#include <QGroupBox>
#include <QBoxLayout>
#include <QDialog>

#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include <QDateTimeEdit>
#include <QLineEdit>
#include <QTextEdit>

#include "ui/tab/StartPanel.hpp"
#include "event/dialog/AddTaskListener.hpp"

class AddTaskListener;
class StartPanel;

class AddTaskDialog final : public QDialog {
    Q_OBJECT
public:
    explicit AddTaskDialog(const QString& title);
    ~AddTaskDialog();

    QCheckBox* getCompleteCheckBox() const;
    QCheckBox* getFavoriteCheckBox() const;

    QDateTimeEdit* getTimestampEdit() const;
    QLineEdit* getNameEdit() const;
    QTextEdit* getDescriptionEdit() const;

    QPushButton* getConfirmButton() const;
    QPushButton* getCancelButton() const;

    StartPanel* getParent() const;
    void setParent(StartPanel* panel);

private:
    void initUI();

    void initFlagRegion();
    void initTimestampRegion();
    void initNameRegion();
    void initDescriptionRegion();

    void initControlRegion();

    AddTaskListener* mListener;
    StartPanel* mPanel;

    QBoxLayout* mVerticalBox;

    QBoxLayout* mCompleteLayout;
    QBoxLayout* mFavoriteLayout;

    QBoxLayout* mTimestampLayout;
    QBoxLayout* mNameLayout;
    QBoxLayout* mDescriptionLayout;

    QBoxLayout* mControlLayout;

    QLabel* mCompleteLabel;
    QLabel* mFavoriteLabel;

    QLabel* mTimestampLabel;
    QLabel* mNameLabel;
    QLabel* mDescriptionLabel;

    QCheckBox* mCompleteCheckBox;
    QCheckBox* mFavoriteCheckBox;

    QDateTimeEdit* mTimestampEdit;
    QLineEdit* mNameEdit;
    QTextEdit* mDescriptionEdit;

    QPushButton* mConfirmButton;
    QPushButton* mCancelButton;
};

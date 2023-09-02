﻿/*
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

#include <QTableView>

#include <QGroupBox>

#include <QPushButton>
#include <QBoxLayout>

#include "event/tab/CompleteEvent.hpp"

#include "model/TaskModel.hpp"

class CompleteEvent;

class CompletePanel final: public QGroupBox {
public:
    explicit CompletePanel(QWidget* parent = nullptr);
    ~CompletePanel();

    QTableView* getTable() const;
    CompleteEvent* getEvent() const;

    void setupModel();
    void updateTable();

    void clearTable();

private:
    void initTable();
    void initButtons();

    void setupHeaderTable();

    TaskModel* mModel;
    CompleteEvent *mEvent;

    QBoxLayout* mLayout;
    QTableView* mTable;

    QPushButton* mUpdateButton;
};

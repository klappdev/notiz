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

#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

#include "event/tab/SettingsListener.hpp"

class SettingsListener;

class SettingsPanel final: public QGroupBox {
    Q_OBJECT
public:
    explicit SettingsPanel(QWidget* parent = nullptr);
    ~SettingsPanel();

    QCheckBox* getLightThemeCheckBox() const;
    QCheckBox* getDarkThemeCheckBox() const;

    QCheckBox* getEnLanguageCheckBox() const;
    QCheckBox* getDeLanguageCheckBox() const;

    QCheckBox* getTurnOnCheckBox() const;
    QCheckBox* getTurnOffCheckBox() const;

    QLabel* getThemeLabel() const;
    QLabel* getLanguageLabel() const;
    QLabel* getNotificationLabel() const;
    QLabel* getResetLabel() const;
    QLabel* getAboutLabel() const;

    SettingsListener* getListener() const;

private:
    void initThemeRegion();
    void initLanguageRegion();
    void initNotificationRegion();
    void initResetRegion();
    void initAboutRegion();
    void initControlRegion();

    SettingsListener* mListener;

    QBoxLayout* mLayout;

    QCheckBox* mLightThemeCheckBox;
    QCheckBox* mDarkThemeCheckBox;

    QCheckBox* mEnLanguageCheckBox;
    QCheckBox* mDeLanguageCheckBox;

    QCheckBox* mTurnOnCheckBox;
    QCheckBox* mTurnOffCheckBox;

    QLabel* mLightThemeLabel;
    QLabel* mDarkThemeLabel;

    QLabel* mEnIconLabel;
    QLabel* mDeIconLabel;

    QLabel* mThemeLabel;
    QLabel* mLanguageLabel;
    QLabel* mNotificationLabel;
    QLabel* mResetLabel;
    QLabel* mAboutLabel;

    QPushButton* mResetButton;
    QPushButton* mAboutButton;
    QPushButton* mApplyButton;
};

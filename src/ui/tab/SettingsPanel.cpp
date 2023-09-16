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

#include "ui/tab/SettingsPanel.hpp"

static constexpr const char* const TAG =  "[SettingsPanel] ";

SettingsPanel::SettingsPanel(QWidget* parent)
    : QGroupBox(parent)
    , mListener(new SettingsListener(this)) {
    setObjectName("settings-panel");

    mLayout = new QVBoxLayout;
    mLayout->setMargin(10);
    mLayout->setSpacing(10);

    initThemeRegion();
    initLanguageRegion();
    initNotificationRegion();
    initResetRegion();
    initAboutRegion();
    initControlRegion();

    setLayout(mLayout);
}

SettingsPanel::~SettingsPanel() {
    delete mListener;
}

void SettingsPanel::initThemeRegion() {
    mThemeLabel = new QLabel(tr("Theme: "));

    mLightThemeLabel = new QLabel;
    mLightThemeLabel->setPixmap(QPixmap(":res/image/theme/light-theme.png"));

    mDarkThemeLabel = new QLabel;
    mDarkThemeLabel->setPixmap(QPixmap(":res/image/theme/dark-theme.png"));

    mLightThemeCheckBox = new QCheckBox(tr("light theme"));
    mLightThemeCheckBox->setObjectName("theme-checkbox");

    mDarkThemeCheckBox = new QCheckBox(tr("dark theme"));
    mDarkThemeCheckBox->setObjectName("theme-checkbox");

    QObject::connect(mLightThemeCheckBox, SIGNAL(clicked(bool)),
                     mListener,           SLOT(chooseLightTheme(bool)));
    QObject::connect(mDarkThemeCheckBox, SIGNAL(clicked(bool)),
                     mListener,          SLOT(chooseDarkTheme(bool)));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addSpacing(50);
    horizontalLayout->addWidget(mLightThemeLabel);
    horizontalLayout->addWidget(mLightThemeCheckBox);
    horizontalLayout->addSpacing(35);
    horizontalLayout->addWidget(mDarkThemeLabel);
    horizontalLayout->addWidget(mDarkThemeCheckBox);
    horizontalLayout->addStretch(1);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    QBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(mThemeLabel);
    verticalLayout->addLayout(horizontalLayout);

    mLayout->addLayout(verticalLayout);
}

void SettingsPanel::initLanguageRegion() {
    mLanguageLabel = new QLabel(tr("Language: "));

    mEnIconLabel = new QLabel;
    mEnIconLabel->setPixmap(QPixmap(":res/image/country/en.png"));

    mDeIconLabel = new QLabel;
    mDeIconLabel->setPixmap(QPixmap(":res/image/country/de.png"));

    mEnLanguageCheckBox = new QCheckBox("en");
    mEnLanguageCheckBox->setObjectName("en-lanugage-checkbox");

    mDeLanguageCheckBox = new QCheckBox("de");
    mDeLanguageCheckBox->setObjectName("de-language-checkbox");

    QObject::connect(mEnLanguageCheckBox, SIGNAL(clicked(bool)),
                     mListener,           SLOT(chooseEnLanguage(bool)));
    QObject::connect(mDeLanguageCheckBox, SIGNAL(clicked(bool)),
                     mListener,           SLOT(chooseDeLanguage(bool)));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addSpacing(50);
    horizontalLayout->addWidget(mEnIconLabel);
    horizontalLayout->addWidget(mEnLanguageCheckBox);
    horizontalLayout->addSpacing(35);
    horizontalLayout->addWidget(mDeIconLabel);
    horizontalLayout->addWidget(mDeLanguageCheckBox);
    horizontalLayout->addStretch(1);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    QBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(mLanguageLabel);
    verticalLayout->addLayout(horizontalLayout);

    mLayout->addLayout(verticalLayout);
}

void SettingsPanel::initNotificationRegion() {
    mNotificationLabel = new QLabel(tr("Notification: "));

    mTurnOnCheckBox = new QCheckBox(tr("Turn on"));
    mTurnOnCheckBox->setObjectName("turn-on-checkbox");

    mTurnOffCheckBox = new QCheckBox(tr("Turn off"));
    mTurnOffCheckBox->setObjectName("turn-off-checkbox");

    QObject::connect(mTurnOnCheckBox, SIGNAL(clicked(bool)),
                     mListener,       SLOT(turnOnNotification(bool)));
    QObject::connect(mTurnOffCheckBox, SIGNAL(clicked(bool)),
                     mListener,        SLOT(turnOffNotification(bool)));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addSpacing(50);
    horizontalLayout->addWidget(mTurnOnCheckBox);
    horizontalLayout->addWidget(mTurnOffCheckBox);
    horizontalLayout->addStretch(1);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    QBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(mNotificationLabel);
    verticalLayout->addLayout(horizontalLayout);

    mLayout->addLayout(verticalLayout);
}

void SettingsPanel::initResetRegion() {
    mResetLabel = new QLabel(tr("Reset data:"));

    mResetButton = new QPushButton;
    mResetButton->setObjectName("reset-button");
    mResetButton->setIcon(QIcon(":res/image/submit/trash-icon.png"));

    QObject::connect(mResetButton, SIGNAL(clicked()),
                     mListener,    SLOT(showResetData()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mResetLabel);
    horizontalLayout->addWidget(mResetButton);
    horizontalLayout->addStretch(1);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    mLayout->addLayout(horizontalLayout);
}

void SettingsPanel::initAboutRegion() {
    mAboutLabel = new QLabel(tr("About application: "));

    mAboutButton = new QPushButton;
    mAboutButton->setObjectName("about-button");
    mAboutButton->setIcon(QIcon(":res/image/submit/info-icon.png"));

    QObject::connect(mAboutButton, SIGNAL(clicked()),
                     mListener,    SLOT(showAboutApp()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mAboutLabel);
    horizontalLayout->addWidget(mAboutButton);
    horizontalLayout->addStretch(1);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    mLayout->addLayout(horizontalLayout);
}

void SettingsPanel::initControlRegion() {
    mApplyButton = new QPushButton;
    mApplyButton->setObjectName("confirm-button");

    QObject::connect(mApplyButton, SIGNAL(clicked()),
                     mListener,    SLOT(storeConfiguration()));

    QBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addStretch(1);
    horizontalLayout->addWidget(mApplyButton);
    horizontalLayout->setMargin(5);
    horizontalLayout->setSpacing(5);

    mLayout->addLayout(horizontalLayout);
}


QCheckBox* SettingsPanel::getLightThemeCheckBox() const {
    return mLightThemeCheckBox;
}

QCheckBox* SettingsPanel::getDarkThemeCheckBox() const {
    return mDarkThemeCheckBox;
}

QCheckBox* SettingsPanel::getEnLanguageCheckBox() const {
    return mEnLanguageCheckBox;
}

QCheckBox* SettingsPanel::getDeLanguageCheckBox() const {
    return mDeLanguageCheckBox;
}

QCheckBox* SettingsPanel::getTurnOnCheckBox() const {
    return mTurnOnCheckBox;
}

QCheckBox* SettingsPanel::getTurnOffCheckBox() const {
    return mTurnOffCheckBox;
}

QLabel* SettingsPanel::getThemeLabel() const {
    return mThemeLabel;
}

QLabel* SettingsPanel::getLanguageLabel() const {
    return mLanguageLabel;
}

QLabel* SettingsPanel::getNotificationLabel() const {
    return mNotificationLabel;
}

QLabel* SettingsPanel::getResetLabel() const {
    return mResetLabel;
}

QLabel* SettingsPanel::getAboutLabel() const {
    return mAboutLabel;
}

SettingsListener* SettingsPanel::getListener() const {
    return mListener;
}

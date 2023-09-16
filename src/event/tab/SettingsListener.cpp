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

#include "event/tab/SettingsListener.hpp"
#include "setting/AppSettings.hpp"

#include "ui/util/UiUtil.hpp"
#include "ui/dialog/AboutAppDialog.hpp"

#include <QMessageBox>

static constexpr const char* const TAG = "[SettingsListener] ";

SettingsListener::SettingsListener(SettingsPanel* parent)
    : mParent(parent) {
}

SettingsListener::~SettingsListener() {}

void SettingsListener::showAboutApp() {
    AboutAppDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        qInfo() << TAG << "Dialog was closed!" << endl;
    }
}

void SettingsListener::showResetData() {
    QMessageBox::StandardButton replyButton;
    replyButton = QMessageBox::question(nullptr,
                                  tr("Reset data"),
                                  tr("Do you agree, that want to delete data?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (replyButton == QMessageBox::Yes) {
        /*
            TaskDB::getInstance().reset();
        */

        qInfo() << "reset all task!" << endl;
    }
}

void SettingsListener::chooseLightTheme(bool checked) {
    if (checked) {
        mParent->getDarkThemeCheckBox()->setChecked(false);
    }
}

void SettingsListener::chooseDarkTheme(bool checked) {
    if (checked) {
        mParent->getLightThemeCheckBox()->setChecked(false);
    }
}

void SettingsListener::chooseEnLanguage(bool checked) {
    if (checked) {
        mParent->getDeLanguageCheckBox()->setChecked(false);
    }
}

void SettingsListener::chooseDeLanguage(bool checked) {
    if (checked) {
        mParent->getEnLanguageCheckBox()->setChecked(false);
    }
}

void SettingsListener::turnOnNotification(bool checked) {
    if (checked) {
        mParent->getTurnOffCheckBox()->setChecked(false);
    }
}

void SettingsListener::turnOffNotification(bool checked) {
    if (checked) {
        mParent->getTurnOnCheckBox()->setChecked(false);
    }
}

void SettingsListener::storeConfiguration() {
    auto& settings = AppSettings::getInstance();

    if (mParent->getTurnOnCheckBox()->isChecked()) {
        settings.setNotified(true);
    } else {
        settings.setNotified(false);
    }

    if (mParent->getEnLanguageCheckBox()->isChecked()) {
        settings.setLanguage("en");
    } else if (mParent->getDeLanguageCheckBox()->isChecked()) {
        settings.setLanguage("de");
    }

    if (mParent->getLightThemeCheckBox()->isChecked()) {
        settings.setTheme("light");
    } else if (mParent->getDarkThemeCheckBox()->isChecked()) {
        settings.setTheme("dark");
    }

    settings.store();

    UiUtil::showInfoMessage(tr("Save configuration"),
                            tr("Configuration data was saved."
                               "Reload application for start"
                               "using new configuration!"));
}

void SettingsListener::loadConfiguration(int index) {
    Q_UNUSED(index);

    auto& settings = AppSettings::getInstance();

    if (settings.isNotified()) {
        mParent->getTurnOnCheckBox()->setChecked(true);
        mParent->getTurnOffCheckBox()->setChecked(false);
    } else {
        mParent->getTurnOnCheckBox()->setChecked(false);
        mParent->getTurnOffCheckBox()->setChecked(true);
    }

    if (settings.getLanguage() == "en") {
        mParent->getEnLanguageCheckBox()->setChecked(true);
        mParent->getDeLanguageCheckBox()->setChecked(false);

    } else if (settings.getLanguage() == "de") {
        mParent->getEnLanguageCheckBox()->setChecked(false);
        mParent->getDeLanguageCheckBox()->setChecked(true);

    }

    if (settings.getTheme() == "light") {
        mParent->getLightThemeCheckBox()->setChecked(true);
        mParent->getDarkThemeCheckBox()->setChecked(false);

    } else if (settings.getTheme() == "dark") {
        mParent->getLightThemeCheckBox()->setChecked(false);
        mParent->getDarkThemeCheckBox()->setChecked(true);
    }
}

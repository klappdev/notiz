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

#include "setting/AppSettings.hpp"

#include <QList>
#include <QDebug>

static constexpr const char* const TAG = "[AppSettings] ";

static const bool DEFAULT_NOTIFIED = true;
static const QString DEFAULT_THEME =  "light";
static const QString DEFAULT_LANGUAGE = "en";

AppSettings::AppSettings()
    : mAppSettings(/*organization=*/ "kl",
                   /*application=*/ "notiz") {
}

AppSettings::~AppSettings() {}

auto AppSettings::getInstance() -> AppSettings& {
    static AppSettings instance;

    return instance;
}

void AppSettings::create() {
    mSettings.notified = DEFAULT_NOTIFIED;
    mSettings.theme = DEFAULT_THEME;
    mSettings.language = DEFAULT_LANGUAGE;

    store();
}

void AppSettings::store() {
    mAppSettings.beginGroup("/settings");

    mAppSettings.setValue("/notified", mSettings.notified);
    mAppSettings.setValue("/theme", mSettings.theme);
    mAppSettings.setValue("/language", mSettings.language);

    mAppSettings.endGroup();
}

void AppSettings::load() {
    mAppSettings.beginGroup("/settings");

    mSettings.notified = mAppSettings.value("/notified", DEFAULT_NOTIFIED).toBool();
    mSettings.theme = mAppSettings.value("/theme", DEFAULT_THEME).toString();
    mSettings.language = mAppSettings.value("/language", DEFAULT_LANGUAGE).toString();

    mAppSettings.endGroup();
}

void AppSettings::setNotified(bool notified) {
    mSettings.notified = notified;
}

bool AppSettings::isNotified() const {
    return mSettings.notified;
}

void AppSettings::setTheme(const QString& theme) {
    mSettings.theme = theme;
}

auto AppSettings::getTheme() const -> const QString& {
    return mSettings.theme;
}

void AppSettings::setLanguage(const QString& language) {
    mSettings.language = language;
}

auto AppSettings::getLanguage() const -> const QString& {
    return mSettings.language;
}

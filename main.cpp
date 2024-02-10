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

#include <QApplication>

#include <QFile>

#include "ui/main/MainWindow.hpp"
#include "ui/dialog/LoginDialog.hpp"

#include "setting/AppSettings.hpp"
#include "i18n/AppTranslator.hpp"

static void loadStyle(QApplication& application) {
    QFile file(":res/style/task_style.qss");

    const bool flag = file.open(QFile::ReadOnly);

    if (!flag) {
        qCritical() << "Can not load qss file!" << '\n';
    }

    const QString styleSheetLines = QLatin1String(file.readAll());

    application.setStyleSheet(styleSheetLines);

    file.close();
}

static void loadLanguage() {
    const QString& language = AppSettings::getInstance().getLanguage();

    qCritical() << "Language: " << language << '\n';

    auto& translator = AppTranslator::getInstance();

    translator.prepareTranslation(language);
}

static void changeTheme(QApplication& application) {
    QPalette palette;

    if (AppSettings::getInstance().getTheme() == "dark") {
        palette.setColor(QPalette::Window, QColorConstants::DarkGray);

    } else if (AppSettings::getInstance().getTheme() == "light") {
        palette.setColor(QPalette::Window, QColorConstants::White);
    }

    application.setPalette(palette);
}

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    AppSettings::getInstance().load();

    loadStyle(application);
    loadLanguage();
    changeTheme(application);

    MainWindow mainWindow;
    mainWindow.hide();

    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Rejected) {
        qCritical() << "Login dialog are closed!" << '\n';
    }

    return application.exec();
}

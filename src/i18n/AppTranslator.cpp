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

#include "i18n/AppTranslator.hpp"
#include "ui/util/UiUtil.hpp"

#include <QCoreApplication>
#include <QTextCodec>

#include <QDir>
#include <QLibraryInfo>

AppTranslator::AppTranslator() {}
AppTranslator::~AppTranslator() {}

auto AppTranslator::getInstance() -> AppTranslator& {
    static AppTranslator instance;

    return instance;
}

void AppTranslator::prepareTranslation(const QString& name) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(encoding.toAscii()));
    QTextCodec::setCodecForTr(QTextCodec::codecForName(encoding.toAscii()));
#endif

    const QString qtTranslationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    const bool qtTranslated = mQtTranslator.load(QString("qt_") + name, qtTranslationPath);

    if (qtTranslated) {
        QCoreApplication::installTranslator(&mQtTranslator);
    } else {
        UiUtil::showErrorMessage("Change language qt app",
                                 "The language was not installed!" + name);
    }

    QCoreApplication::removeTranslator(&mTranslator);

    const QString appTranslatePath =  ":/res/i18n/";
    const bool appTranslated = mTranslator.load(QString("notiz_") + name, appTranslatePath);

    if (appTranslated) {
        QCoreApplication::installTranslator(&mTranslator);
    } else {
        UiUtil::showErrorMessage("Change language app",
                                 "The language was not installed!");
    }
}

auto AppTranslator::getSupportLanguages() -> QList<QString> {
    QList<QString> list;

    QDir folder(":res/i18n/");
    QList<QString> files = folder.entryList(QDir::NoDotAndDotDot | QDir::Files);

    for (const QString& name : files) {
        if (name.endsWith(".qm")) {
            list.append(name.split("_").last().
                             split(".").first());
        }
    }

    return list;
}

auto AppTranslator::searchLanguages(const QString& name) -> QList<QString> {
    QList<QString> list;

    for (const QString& current : getSupportLanguages()) {
        if (current.contains(name)) {
            list.append(current);
        }
    }

    return list;
}

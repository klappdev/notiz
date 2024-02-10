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

#include "ui/main/MainWindow.hpp"
#include "ui/tab/FavoritePanel.hpp"
#include "ui/tab/CompletePanel.hpp"
#include "ui/tab/CalendarPanel.hpp"
#include "ui/tab/SettingsPanel.hpp"

#include "ui/util/UiUtil.hpp"
#include "ui/dialog/LoginDialog.hpp"
#include "setting/AppSettings.hpp"

#include <QApplication>
#include <QLabel>
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
#include <QDesktopWidget>
#else
#include <QScreen>
#endif

static constexpr const char* const TAG =  "[MainWindow] ";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mTimer(new QTimer()) {
    setObjectName("main-window");
    setWindowTitle(tr("Notice"));

    setWindowIcon(QIcon(":res/image/task_icon.png"));
    setCenter();

    mMainWidget = new QWidget;

    initTabs();
    initTimer();

    mMainLayout = new QHBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    mMainLayout->setMargin(5);
#else
    mMainLayout->setContentsMargins(5, 5, 5, 5);
#endif
    mMainLayout->setSpacing(5);

    mMainLayout->addWidget(mTabs);

    mMainWidget->setLayout(mMainLayout);
    setCentralWidget(mMainWidget);

    resize(800, 500);
}

MainWindow::~MainWindow() {
    delete mTimer;
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);

    if (AppSettings::getInstance().isNotified()) {
        mTimer->start();
    }
}

void MainWindow::closeEvent(QCloseEvent* /*event*/) {
    if (AppSettings::getInstance().isNotified()) {
        mTimer->stop();
    }
}

void MainWindow::initTimer() {
    mTimer->setInterval(60 * 1000);   /* 1m */

    QObject::connect(mTimer, SIGNAL(timeout()),
                     this,   SLOT(notifyTasks()));
}

void MainWindow::initTabs() {
    this->mTabs = new QTabWidget;

    mTabs->addTab(new QLabel("", mTabs), tr("Main"));
    mTabs->addTab(new QLabel("", mTabs), tr("Favorite"));
    mTabs->addTab(new QLabel("", mTabs), tr("Complete"));
    mTabs->addTab(new QLabel("", mTabs), tr("Calendar"));
    mTabs->addTab(new QLabel("", mTabs), tr("Settings"));

    initStartPage(PageTab::START_PAGE);
    initFavoritePage(PageTab::FAVORITE_PAGE);
    initCompletePage(PageTab::COMPLETE_PAGE);
    initCalendarPage(PageTab::CALENDAR_PAGE);
    initSettingsPage(PageTab::SETTING_PAGE);
}

void MainWindow::initStartPage(PageTab tab) {
    QWidget* startPage = mTabs->widget(static_cast<int>(tab));
    mStartPanel = new StartPanel;

    QBoxLayout* verticalBox = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    verticalBox->setMargin(5);
#else
    verticalBox->setContentsMargins(5, 5, 5, 5);
#endif
    verticalBox->setSpacing(5);

    verticalBox->addWidget(mStartPanel);

    startPage->setLayout(verticalBox);
}

void MainWindow::initFavoritePage(PageTab tab) {
    QWidget* favoritePage = mTabs->widget(static_cast<int>(tab));

    auto* favoritePanel = new FavoritePanel;

    QObject::connect(mTabs,  SIGNAL(tabBarClicked(int)),
                     favoritePanel, SLOT(updateTasks(int)));

    QBoxLayout* verticalBox = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    verticalBox->setMargin(5);
#else
    verticalBox->setContentsMargins(5, 5, 5, 5);
#endif
    verticalBox->setSpacing(5);
    verticalBox->addWidget(favoritePanel);

    favoritePage->setLayout(verticalBox);
}

void MainWindow::initCompletePage(PageTab tab) {
    QWidget* completePage = mTabs->widget(static_cast<int>(tab));

    auto* completePanel = new CompletePanel;

    QObject::connect(mTabs,  SIGNAL(tabBarClicked(int)),
                     completePanel, SLOT(updateTasks(int)));

    QBoxLayout* verticalBox = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    verticalBox->setMargin(5);
#else
    verticalBox->setContentsMargins(5, 5, 5, 5);
#endif
    verticalBox->setSpacing(5);
    verticalBox->addWidget(completePanel);

    completePage->setLayout(verticalBox);
}

void MainWindow::initCalendarPage(PageTab tab) {
    QWidget* calendarPage = mTabs->widget(static_cast<int>(tab));

    CalendarPanel* calendarPanel = new CalendarPanel;

    connect(mTabs,  SIGNAL(tabBarClicked(int)),
            calendarPanel, SLOT(updateCalendar(int)));

    QBoxLayout* verticalBox = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    verticalBox->setMargin(5);
#else
    verticalBox->setContentsMargins(5, 5, 5, 5);
#endif
    verticalBox->setSpacing(5);
    verticalBox->addWidget(calendarPanel);

    calendarPage->setLayout(verticalBox);
}

void MainWindow::initSettingsPage(PageTab tab) {
    QWidget* settingsPage = mTabs->widget(static_cast<int>(tab));

    auto* settingsPanel = new SettingsPanel;

    connect(mTabs,  SIGNAL(tabBarClicked(int)),
            settingsPanel, SLOT(loadConfiguration(int)));

    QBoxLayout* verticalBox = new QVBoxLayout;
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    verticalBox->setMargin(5);
#else
    verticalBox->setContentsMargins(5, 5, 5, 5);
#endif
    verticalBox->setSpacing(5);

    verticalBox->addWidget(settingsPanel);

    settingsPage->setLayout(verticalBox);
}

void MainWindow::setCenter() {
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    const QRect rectangle = QApplication::desktop()->availableGeometry(this);
#else
    const QRect rectangle = QGuiApplication::primaryScreen()->availableGeometry();
#endif
    const QPoint center = rectangle.center();

    move(center.x() - width() * 0.5, center.y() - height() * 0.5);
}

void MainWindow::notifyTasks() {
    const QString format = "dd-MM-yyyy_hh:mm";
    const QList<Task>& tasks = mStartPanel->getModel()->getTasks();

    for (const Task& task : tasks) {
        /* without seconds */
        const QString currentDateTime = QDateTime::currentDateTime().toString(format);
        const QString taskDateTime = task.timestamp.left(task.timestamp.lastIndexOf(':'));

        if (currentDateTime == taskDateTime) {
            UiUtil::showInfoMessage(tr("Notify task"),
                                    tr("Start notify task: %1\n").arg(task.name));
        }
    }
}

void MainWindow::exit() {
    auto* loginDialog = static_cast<LoginDialog*>(UiUtil::getLoginWindow());
    Q_CHECK_PTR(loginDialog);
    AppSettings::getInstance().store();

    loginDialog->reject();
    qApp->quit();
}

StartPanel* MainWindow::getStartPanel() const {
    return mStartPanel;
}

QTabWidget* MainWindow::getTabs() const {
    return mTabs;
}

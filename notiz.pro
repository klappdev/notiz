QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notiz
TEMPLATE = app

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH += include

HEADERS  += include/ui/main/MainWindow.hpp          \
        include/ui/main/PageTab.hpp                 \
        include/ui/util/UiUtil.hpp                  \
                                                    \
        include/ui/widget/TaskCalendar.hpp          \
        include/ui/tab/StartPanel.hpp               \
        include/ui/tab/FavoritePanel.hpp            \
        include/ui/tab/CompletePanel.hpp            \
        include/ui/tab/CalendarPanel.hpp            \
        include/ui/tab/SettingsPanel.hpp            \
                                                    \
        include/ui/dialog/AboutAppDialog.hpp        \
        include/ui/dialog/CalendarDialog.hpp        \
        include/ui/dialog/LoginDialog.hpp           \
        include/ui/dialog/AddTaskDialog.hpp         \
        include/ui/dialog/EditTaskDialog.hpp        \
                                                    \
        include/event/dialog/AddTaskEvent.hpp       \
        include/event/dialog/LoginEvent.hpp         \
        include/event/dialog/EditTaskEvent.hpp      \
        include/event/dialog/CalendarTaskEvent.hpp  \
                                                    \
        include/event/tab/SettingsEvent.hpp         \
        include/event/tab/CalendarEvent.hpp         \
        include/event/tab/StartEvent.hpp            \
        include/event/tab/FavoriteEvent.hpp         \
        include/event/tab/CompleteEvent.hpp         \
        include/event/ControlEvent.hpp              \
                                                    \
        include/model/TaskModel.hpp                 \
                                                    \
        include/setting/AppSettings.hpp             \
                                                    \
        include/db/TaskDao.hpp                      \
        include/db/AccountDao.hpp                   \
                                                    \
        include/i18n/AppTranslator.hpp              \
                                                    \
        include/common/Settings.hpp                 \
        include/common/Task.hpp                     \
        include/common/Account.hpp


SOURCES += main.cpp                             \
        src/ui/main/MainWindow.cpp              \
        src/ui/util/UiUtil.cpp                  \
                                                \
        src/ui/widget/TaskCalendar.cpp          \
        src/ui/tab/StartPanel.cpp               \
        src/ui/tab/FavoritePanel.cpp            \
        src/ui/tab/CompletePanel.cpp            \
        src/ui/tab/CalendarPanel.cpp            \
        src/ui/tab/SettingsPanel.cpp            \
                                                \
        src/ui/dialog/CalendarDialog.cpp        \
        src/ui/dialog/LoginDialog.cpp           \
        src/ui/dialog/AddTaskDialog.cpp         \
        src/ui/dialog/EditTaskDialog.cpp        \
        src/ui/dialog/AboutAppDialog.cpp        \
                                                \
        src/event/dialog/AddTaskEvent.cpp       \
        src/event/dialog/EditTaskEvent.cpp      \
        src/event/dialog/LoginEvent.cpp         \
        src/event/dialog/CalendarTaskEvent.cpp  \
                                                \
        src/event/tab/CalendarEvent.cpp         \
        src/event/tab/StartEvent.cpp            \
        src/event/tab/FavoriteEvent.cpp         \
        src/event/tab/CompleteEvent.cpp         \
        src/event/tab/SettingsEvent.cpp         \
        src/event/ControlEvent.cpp              \
                                                \
        src/model/TaskModel.cpp                 \
                                                \
        src/setting/AppSettings.cpp             \
                                                \
        src/db/TaskDao.cpp                      \
        src/db/AccountDao.cpp                   \
                                                \
        src/i18n/AppTranslator.cpp


RESOURCES += main.qrc

TRANSLATIONS += res/i18n/notiz_en.ts            \
                res/i18n/notiz_de.ts

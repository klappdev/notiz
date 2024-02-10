
QT += widgets core gui sql
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

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

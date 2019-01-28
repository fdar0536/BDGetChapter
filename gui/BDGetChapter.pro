QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BDGetChapter
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        utils.cpp \
        ../common/bdgetchap.cpp

HEADERS += \
        mainwindow.h \
        utils.h \
        ../common/bdgetchap.h

win32 {
    INCLUDEPATH += "M:/path/to/your/libbluray/include"
    LIBS += "M:/path/to/your/libbluray/lib/x64/bluray.lib" \
            "M:/path/to/your/libbluray/lib/x64/libbluray.lib"
}

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
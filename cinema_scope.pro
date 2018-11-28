#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CinemaScope
TEMPLATE = app

# debug/release
CONFIG(debug, debug|release) {
    DESTDIR = build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = build/release
}
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += console


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/CinDBReader.cpp \
        src/CinParamSliders.cpp \
        src/CinParameter.cpp \
        src/CinParamSet.cpp \
        src/CinDatabase.cpp \
        src/CinImageViewer.cpp \
        src/CinArtifactSet.cpp \
        src/CinDBView.cpp \
        src/CinDBFactory.cpp \
        src/CinCore.cpp

HEADERS += \
        src/mainwindow.h \
        src/CinDBReader.h \
        src/CinParamSliders.h \
        src/CinParameter.h \
        src/CinParamSet.h \
        src/CinDatabase.h \
        src/CinImageViewer.h \
        src/CinArtifactSet.h \
        src/CinDBView.h \
        src/CinDBFactory.h \
        src/CinCore.h

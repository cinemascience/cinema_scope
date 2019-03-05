#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG += debug_and_release

# TARGET = EmuTracker

# debug/release
CONFIG(release, debug|release) {
    DESTDIR = build/release
} else {
    DESTDIR = build/debug
}
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u

LIBS += -L/opt/local/lib -lgsl -lgslcblas

INCLUDEPATH += /opt/local/include

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
        src/CinScopeWindow.cpp \
        src/CinDBReader.cpp \
        src/CinParamSliders.cpp \
        src/CinParameter.cpp \
        src/CinParamSet.cpp \
        src/CinDatabase.cpp \
        src/CinImageView.cpp \
        src/CinLinechartWidget.cpp \
        src/CinArtifactSet.cpp \
        src/CinDBView.cpp \
        src/CinCore.cpp \
        src/CinParameterMapDialog.cpp \
        src/CinCompoundSlider.cpp \
        src/EmuEmulator.cpp \
        src/EmuLoader.cpp \
        src/EmuResult.cpp \
        src/EmuDatabase.cpp \
        src/EmuSlider.cpp \
        src/EmuInputSliders.cpp \
        src/EmuInputPanel.cpp \
        src/test.emu/emu.c

HEADERS += \
        src/CinScopeWindow.h \
        src/CinDBReader.h \
        src/CinParamSliders.h \
        src/CinParameter.h \
        src/CinParamSet.h \
        src/CinDatabase.h \
        src/CinImageView.h \
        src/CinLinechartWidget.h \
        src/CinArtifactSet.h \
        src/CinDBView.h \
        src/CinCore.h \
        src/CinParameterMapDialog.h \
        src/CinCompoundSlider.h \
        src/EmuEmulator.h \
        src/EmuLoader.h \
        src/EmuResult.h \
        src/EmuDatabase.h \
        src/EmuSlider.h \
        src/EmuInputSliders.h \
        src/EmuInputPanel.h \
        src/test.emu/params.h


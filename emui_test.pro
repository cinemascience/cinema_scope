QT += testlib
QT += gui

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  src/EmuTest.cpp \
            src/EmuDatabase.cpp \
            src/EmuResult.cpp

HEADERS +=  src/EmuDatabase.h \
            src/EmuResult.h

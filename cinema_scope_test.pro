QT += testlib
QT += gui
QT += sql

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  src/CinParameter.cpp \
            src/CinDBReader.cpp \
            src/CinScopeTest.cpp \
            src/CinParameterMap.cpp

HEADERS +=  src/CinParameter.h \
            src/CinDBReader.h \
            src/CinParameterMap.h

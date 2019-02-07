QT += testlib
QT += gui
QT += sql

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  src/CinParameter.cpp \
            src/CinParamSet.cpp \
            src/CinDBReader.cpp \
            src/CinDBView.cpp \
            src/CinScopeTest.cpp \
            src/CinDatabase.cpp \
            src/CinArtifactSet.cpp \
            src/CinCore.cpp

HEADERS +=  src/CinParameter.h \
            src/CinParamSet.h \
            src/CinDBReader.h \
            src/CinDBView.h \
            src/CinDatabase.h \
            src/CinArtifactSet.h \
            src/CinCore.h

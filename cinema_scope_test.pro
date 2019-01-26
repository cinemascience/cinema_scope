QT += testlib
QT += gui
QT += sql

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  src/CinParameter.cpp \
            src/CinParameterVariant.cpp \
            src/CinDBReader.cpp \
            src/CinScopeTest.cpp \
            src/CinDatabase.cpp \
            src/CinDBView.cpp \
            src/CinParamSet.cpp \
            src/CinArtifactSet.cpp \
            src/CinCore.cpp

HEADERS +=  src/CinParameter.h \
            src/CinParameterVariant.h \
            src/CinDBReader.h \
            src/CinDatabase.h \
            src/CinDBView.h \
            src/CinParamSet.h \
            src/CinArtifactSet.h \
            src/CinCore.h

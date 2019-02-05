QT += testlib
QT += gui
QT += sql

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  src/CinParameter.cpp \
            src/CinParamSet.cpp \
            src/CinParameterVariant.cpp \
            src/CinCompoundSlider.cpp \
            src/CinDBReader.cpp \
            src/CinScopeTest.cpp \
            src/CinDatabase.cpp \
            src/CinParamSet.cpp \
            src/CinArtifactSet.cpp \
            src/CinCore.cpp

HEADERS +=  src/CinParameter.h \
            src/CinParamSet.h \
            src/CinParameterVariant.h \
            src/CinCompoundSlider.h \
            src/CinDBReader.h \
            src/CinDatabase.h \
            src/CinParamSet.h \
            src/CinArtifactSet.h \
            src/CinCore.h

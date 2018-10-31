QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = src/cinemascope.h
SOURCES       = src/cinemascope.cpp \
                src/main.cpp


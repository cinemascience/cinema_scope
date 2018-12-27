#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        ;;
    osx)
        brew install qt5
        ;;
    windows)
        # Download and run qt installer
        # (Thanks https://github.com/sgsaenger/vipster for example!)
        wget "http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe" -O qt_installer.exe
        ./qt_installer.exe --verbose --script $TRAVIS_BUILD_DIR/travis/navigate_qt_installer.qs
        # Add Mingw and qt to path
        export MWDIR="/c/Users/travis/Qt/Tools/mingw730_64"
        export QTDIR="/c/Users/travis/Qt/5.12.0/mingw73_64"
        export PATH="$MWDIR/bin:$QTDIR/bin:$PATH"
        ;;
esac
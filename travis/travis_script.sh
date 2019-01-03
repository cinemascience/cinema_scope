#!/usr/bin/bash
set -x

case $TRAVIS_OS_NAME in
    linux)
        qmake cinema_scope.pro
        make
        . $TRAVIS_BUILD_DIR/travis/build_installer.sh
        ;;
    osx)
        if [["$osx_image" == "xcode10"]]; then
            qmake -spec macx-xcode cinema_scope.pro
            xcodebuild -project cinema_scope.xcodeproj
        else
            qmake cinema_scope.pro
            make
        fi
        . $TRAVIS_BUILD_DIR/travis/build_installer.sh
        ;;
    windows)
        qmake cinema_scope.pro
        mingw32-make
        . $TRAVIS_BUILD_DIR/travis/build_installer.sh
        ;;
esac
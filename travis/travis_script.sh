#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        qmake cinema_scope.pro
        make
        ;;
    osx)
        if [["$osx_image" == "xcode10"]]; then
            qmake -spec macx-xcode cinema_scope.pro
            xcodebuild -project cinema_scope.xcodeproj
        else
            qmake cinema_scope.pro
            make
        fi
        ;;
    windows)
        ;;
esac
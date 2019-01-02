#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        cp -rf build/release/cinema_scope installer/packges/data
        binarycreator -c installer/config/config.xml -p installer/packages cinema_scope_installer
        ;;
    osx)
        ;;
    windows)
        cp -rf build/release/cinema_scope.exe installer/packages/data
        binarycreator -c installer/config/config.xml -p installer/packages cinema_scope_installer.exe
        ;;
esac
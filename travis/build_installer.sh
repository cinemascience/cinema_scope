#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        ;;
    osx)
        ;;
    windows)
        cp -rf build/release/cinema_scope.exe installer/packages/data
        binarycreator -c installer/config/config.xml -p installer/packages cinema_scope_installer.exe
        ;;
esac
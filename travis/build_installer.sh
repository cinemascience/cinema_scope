#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        cp -rf $TRAVIS_BUILD_DIR/build/release/cinema_scope $TRAVIS_BUILD_DIR/installer/packges/data
        binarycreator -c $TRAVIS_BUILD_DIR/installer/config/config.xml -p $TRAVIS_BUILD_DIR/installer/packages cinema_scope_installer
        ;;
    osx)
        ;;
    windows)
        cp -rf build/release/cinema_scope.exe installer/packages/data
        binarycreator -c installer/config/config.xml -p installer/packages cinema_scope_installer.exe
        ;;
esac
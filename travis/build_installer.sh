#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        cp -rf $TRAVIS_BUILD_DIR/build/release/cinema_scope $TRAVIS_BUILD_DIR/installer/packages/data
        binarycreator -c $TRAVIS_BUILD_DIR/installer/config/config.xml -p $TRAVIS_BUILD_DIR/installer/packages cinema_scope_installer
        if [ -e cinema_scope_installer ]
        then
            echo "Installer built!"
        else
            echo "Could not find installer file after building it. Failed!"
            exit 1
        fi
        ;;
    osx)
        cp -rf $TRAVIS_BUILD_DIR/build/release/cinema_scope.app $TRAVIS_BUILD_DIR/installer/packages/data
        binarycreator -c $TRAVIS_BUILD_DIR/installer/config/config.xml -p $TRAVIS_BUILD_DIR/installer/packages cinema_scope_installer.dmg
        if [ -e cinema_scope_installer.dmg ]
        then
            echo "Installer built!"
        else
            echo "Could not find installer file after building it. Failed!"
            exit 1
        fi
        ;;
    windows)
        cp -rf build/release/cinema_scope.exe installer/packages/data
        binarycreator -c installer/config/config.xml -p installer/packages cinema_scope_installer.exe
        if [ -e cinema_scope_installer.exe ]
        then
            echo "Installer built!"
        else
            echo "Could not find installer file after building it. Failed!"
            exit 1
        fi
        ;;
esac
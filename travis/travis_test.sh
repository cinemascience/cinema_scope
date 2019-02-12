#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        echo "*******"
        ls $TRAVIS_BUILD_DIR/build
        echo "*******"
        find $TRAVIS_BUILD_DIR/build/debug -name "*test*"
        ;;
    osx)
        ;;
    windows)
        ;;
esac

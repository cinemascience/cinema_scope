#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        echo "*******"
        ls $TRAVIS_BUILD_DIR/build
        echo "*******"
        $TRAVIS_BUILD_DIR/build/debug/bin/cinema_scope_test
        ;;
    osx)
        ;;
    windows)
        ;;
esac

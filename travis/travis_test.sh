#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        echo *******
        ls $TRAVIS_BUILD_DIR
        echo *******
        $TRAVIS_BUILD_DIR/cinema_scope_test/bin/cinema_scope_test
        ;;
    osx)
        ;;
    windows)
        ;;
esac

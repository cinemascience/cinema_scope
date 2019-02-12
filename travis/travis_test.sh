#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        echo "*******"
        ls -l $TRAVIS_BUILD_DIR/build/debug
        echo "*******"
        #xvfb-run $TRAVIS_BUILD_DIR/cinema_scope_test
        ;;
    osx)
        ;;
    windows)
        ;;
esac

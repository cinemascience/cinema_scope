#!/usr/bin/bash -x
set -e

case $TRAVIS_OS_NAME in
    linux)
        # cd to a next level directory because of the relative path in /src/CinScopeTest.cpp
        cd $TRAVIS_BUILD_DIR/src
        # find the test executable, run it, and quit so it's not run more than once. xargs is used to get the exit code of the tests
        find ../ -name "cinema_scope_test" -print0 -quit | xargs -0 bash -c 2>&1
        cd $TRAVIS_BUILD_DIR
        ;;
    osx)
        cd $TRAVIS_BUILD_DIR/src
        find ../ -name "cinema_scope_test" -print0 -quit | xargs -0 bash -c 2>&1
        cd $TRAVIS_BUILD_DIR
        ;;
    windows)
        cd $TRAVIS_BUILD_DIR/src
        find ../ -name "cinema_scope_test" -print0 -quit | xargs -0 cmd /c 2>&1
        cd $TRAVIS_BUILD_DIR
        ;;
esac

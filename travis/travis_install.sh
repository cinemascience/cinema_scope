#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        ;;
    osx)
        then brew install qt5
        ;;
    windows)
        ;;
esac
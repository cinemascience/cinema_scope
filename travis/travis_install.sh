#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        ;;
    osx)
        brew install qt5
        ;;
    windows)
        ;;
esac
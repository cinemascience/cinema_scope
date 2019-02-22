#!/usr/bin/bash

case $TRAVIS_OS_NAME in
    linux)
        source /opt/qt511/bin/qt511-env.sh
        ;;
    osx)
        ;;
    windows)
        ;;
esac

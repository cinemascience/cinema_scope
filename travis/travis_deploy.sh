#!/usr/bin/bash
set -e

case $TRAVIS_OS_NAME in
    linux)
        ;;
    osx)
        if [[ "$osx_image" == "xcode10" ]]; then
          git clone "https://github.com/EthanS94/cinema_scope"
          cd cinema_scope
          doxygen ./doxyfile.cfg
          git commit -am "Updating doxygen"
          git subtree split --prefix docs -b gh-pages
          git checkout gh-pages
          git push --force "https://${GITHUB_TOKEN}@github.com/EthanS94/cinema_scope" HEAD:gh-pages
        fi
        ;;
    windows)
        ;;
esac

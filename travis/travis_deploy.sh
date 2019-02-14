#!/usr/bin/bash -x
set -e

case $TRAVIS_OS_NAME in
    linux)
        ;;
    osx)
        if [[ "$osx_image" == "xcode10" ]]; then
          cd $TRAVIS_BUILD_DIR
          doxygen ./doxyfile.cfg
          git commit -am "Updating doxygen"
          git subtree split --prefix docs -b gh-pages
          git checkout gh-pages
          git push "https://${GITHUB_TOKEN}@github.com/EthanS94/cinema_scope" HEAD:gh-pages
        fi
        ;;
    windows)
        ;;
esac

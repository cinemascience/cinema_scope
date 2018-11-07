## Cinema Scope v0.1 cross-platform application

Note that Cinema:Scope is in pre-release development. Code should be considered
for experimental use only.

<img src="doc/img/cinema_scope.png" width="350">

This is a basic Cinema database viewer for image-based databases. It is
a cross-platform Qt application, requiring the use of `qmake` to build it.

### Building the application

The application is built on the open source release of Qt, v5.11. To build:

1. Install [qt v5.11](https://www.qt.io/download-qt-installer?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5) 
2. Run `qmake` on the `cinema_scope.pro` for your specific OS and
development environment. Examples include:
    - OSX: `qmake cinema_scope.pro`
    - OSX (XCode): `qmake -spec macx-xcode cinema_scope.pro`
    - Linux: `qmake cinema_scope.pro`
3. You will now have makefiles, etc for your specific platform (assuming the
appropriate tools are installed). Edit the code and run `make` to create
executables.

## Contacts

Contact us at `cinema_dev@lanl.gov` with questions. Issues may be added to the
github project at: https://github.com/cinemascience/cinema_scope.

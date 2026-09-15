# SPDX-License-Identifier: MIT
TEMPLATE = subdirs

SUBDIRS += ./plugin
SUBDIRS += ./src
SUBDIRS += ./qml

desktop.files = harbour-stumblefish-companion.desktop
desktop.path = /usr/share/applications
INSTALLS += desktop

DISTFILES += \
    harbour-stumblefish-companion.desktop

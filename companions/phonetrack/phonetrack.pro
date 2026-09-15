# SPDX-License-Identifier: MIT
TEMPLATE = subdirs

SUBDIRS += ./daemon
SUBDIRS += ./qml

include(../companions.pri)

phonetrackini.path = /usr/share/harbour-stumblefish/lib/phonetrack/
phonetrackini.files += phonetrack.ini
INSTALLS += phonetrackini

DISTFILES += phonetrack.ini


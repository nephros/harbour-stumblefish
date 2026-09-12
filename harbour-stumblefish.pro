# SPDX-License-Identifier: MIT
TARGET = harbour-stumblefish
TEMPLATE = subdirs

SUBDIRS += daemon
SUBDIRS += src
SUBDIRS += tests

phonetrack {
  message("Building $$TARGET with PhoneTrack support.")

  QMAKE_CXXFLAGS += -Werror -Wall
  CONFIG += warn_on
  DISTFILES += common/phonetrack.ini

  SUBDIRS += companions/phonetrack/daemon

  phonetrackini.path = /usr/share/harbour-stumblefish/lib/phonetrack/
  phonetrackini.files += common/phonetrack.ini
  INSTALLS += phonetrackini
}

OTHER_FILES += \
    rpm/harbour-stumblefish.spec \
    rpm/harbour-stumblefish.changes

# SPDX-License-Identifier: MIT
TARGET = harbour-stumblefish
TEMPLATE = subdirs

SUBDIRS += daemon
SUBDIRS += src
SUBDIRS += tests

jollapass {
  message("Building $$TARGET with Glassfish support.")
  DEFINES += FIND_JOLLA_BUDDIES
  SUBDIRS += companions/jollapass
}

glassfish {
  message("Building $$TARGET with Glassfish support.")
  SUBDIRS += companions/glassfish
  DEFINES += GLASSFISH
}

phonetrack {
  message("Building $$TARGET with PhoneTrack support.")

  QMAKE_CXXFLAGS += -Werror -Wall
  CONFIG += warn_on
  DISTFILES += companions/phonetrack/config/phonetrack.ini

  SUBDIRS += companions/phonetrack

}

OTHER_FILES += \
    rpm/harbour-stumblefish.spec \
    rpm/harbour-stumblefish.changes

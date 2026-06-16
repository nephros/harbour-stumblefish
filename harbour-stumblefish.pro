# SPDX-License-Identifier: MIT
TARGET = harbour-stumblefish
TEMPLATE = subdirs

SUBDIRS += daemon
SUBDIRS += src
SUBDIRS += tests

if(GLASSFISH) {
  SUBDIRS += glassfish
}
OTHER_FILES += \
    rpm/harbour-stumblefish.spec \
    rpm/harbour-stumblefish.changes

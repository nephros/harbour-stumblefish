TEMPLATE = subdirs

SUBDIRS += client
SUBDIRS += systemd

QMAKE_CXXFLAGS += -Werror -Wall
CONFIG += warn_on

jollapass {
  message("Building with JollaPass support.")
  DEFINES += FIND_JOLLA_BUDDIES
  SUBDIRS += jollapass
}

glassfish {
  message("Building with Glassfish support.")
  DEFINES += FIND_KLABAUTERS
  SUBDIRS += glassfish
}

phonetrack {
  message("Building with PhoneTrack support.")
  DEFINES += TRACK_MY_PHONE
  SUBDIRS += phonetrack

}



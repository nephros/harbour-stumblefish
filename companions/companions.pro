TEMPLATE = subdirs

jollapass {
  message("Building with JollaPass support.")
  DEFINES += FIND_JOLLA_BUDDIES
  SUBDIRS += jollapass
  SUBDIRS += client
}

glassfish {
  message("Building with Glassfish support.")
  DEFINES += FIND_KLABAUTERS
  SUBDIRS += glassfish
  SUBDIRS += client
}

phonetrack {
  message("Building with PhoneTrack support.")
  DEFINES += TRACK_MY_PHONE
  SUBDIRS += phonetrack
  SUBDIRS += client

  QMAKE_CXXFLAGS += -Werror -Wall
  CONFIG += warn_on
}



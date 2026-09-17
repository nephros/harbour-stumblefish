# SPDX-License-Identifier: MIT
TARGET = harbour-stumblefish-companion

message("Building the companion app.")

CONFIG += sailfishapp
CONFIG += sailfishapp_no_deploy_qml

CONFIG += c++11 link_pkgconfig
QT += dbus qml quick
QT -= gui

PKGCONFIG += sailfishapp
PKGCONFIG += sailfishsilica

STUMBLEFISH_ROOT_DIR = ../../../

INCLUDEPATH += . $${STUMBLEFISH_ROOT_DIR} $${STUMBLEFISH_ROOT_DIR}/src $${STUMBLEFISH_ROOT_DIR}/common

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

jollapass {
  DEFINES += FIND_JOLLA_BUDDIES
}

glassfish {
  DEFINES += FIND_KLABAUTERS
}

phonetrack {
  DEFINES += TRACK_MY_PHONE
  INCLUDEPATH += ../..
  SOURCES += ../../phonetrack/config/phonetrackconfig.cpp
  HEADERS += ../../phonetrack/config/phonetrackconfig.h
}

SOURCES += \
    src/main.cpp \
    src/stumblefishcompanionclient.cpp \
    $${STUMBLEFISH_ROOT_DIR}src/stumblefishclient.cpp

HEADERS += \
    src/stumblefishcompanionclient.h \
    $${STUMBLEFISH_ROOT_DIR}/common/constants.h \
    $${STUMBLEFISH_ROOT_DIR}src/stumblefishclient.h

lupdate_only {
    SOURCES += $$files(qml/*.qml)
}

INSTALLS += qml

target.path = /usr/bin

qml.files += qml/$${TARGET}.qml
qml.path   = /usr/share/harbour-stumblefish/qml

SAILFISHAPP_ICONS = 108x108 128x128 172x172 86x86

DISTFILES += \
    harbour-stumblefish-companion.desktop \
    qml/PhoneTrackSettings.qml \
    qml/PhoneTrackStats.qml \
    icons/icons-glassfish/108x108/apps/harbour-stumblefish.png \
    icons/icons-glassfish/128x128/apps/harbour-stumblefish.png \
    icons/icons-glassfish/172x172/apps/harbour-stumblefish.png \
    icons/icons-glassfish/86x86/apps/harbour-stumblefish.png \
    icons/icons-jollapass/108x108/apps/harbour-stumblefish.png \
    icons/icons-jollapass/128x128/apps/harbour-stumblefish.png \
    icons/icons-jollapass/172x172/apps/harbour-stumblefish.png \
    icons/icons-jollapass/86x86/apps/harbour-stumblefish.png \
    icons/icons-trackfish/108x108/apps/harbour-stumblefish.png \
    icons/icons-trackfish/128x128/apps/harbour-stumblefish.png \
    icons/icons-trackfish/172x172/apps/harbour-stumblefish.png \
    icons/icons-trackfish/86x86/apps/harbour-stumblefish.png

QMAKE_RPATHDIR += /usr/share/$${TARGET}/lib


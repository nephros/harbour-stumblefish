# SPDX-License-Identifier: MIT
TARGET = harbour-stumblefish-companion

message("Building the companion app.")

CONFIG += sailfishapp_no_deploy_qml

CONFIG += c++11 link_pkgconfig
QT += dbus qml quick
PKGCONFIG += sailfishapp

include(../../companions.pri)
# from ../../companions.pri: not used here:
HEADERS -= $${COMPANIONS_BASE_DIR}/companionbase.h
SOURCES -= $${COMPANIONS_BASE_DIR}/companionbase.cpp

INCLUDEPATH += . $${STUMBLEFISH_ROOT_DIR}/src

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

jollapass {
  DEFINES += FIND_JOLLA_BUDDIES
}

glassfish {
  DEFINES += FIND_KLABAUTERS
}

phonetrack {
  DEFINES += TRACK_MY_PHONE
}

SOURCES += \
    src/main.cpp \
    src/stumblefishcompanionclient.cpp

HEADERS += \
    src/stumblefishcompanionclient.h \
    $${STUMBLEFISH_ROOT_DIR}/common/constants.h

lupdate_only {
    SOURCES += $$files(qml/*.qml)
}

INSTALLS += target qml desktop

target.path = /usr/bin

qml.files += qml/$${TARGET}.qml
qml.path   = /usr/share/harbour-stumblefish/qml

desktop.files = $${TARGET}.desktop
desktop.path = /usr/share/applications

DISTFILES += \
    harbour-stumblefish-companion.desktop \
    qml/PhoneTrackSettings.qml \
    qml/PhoneTrackStats.qml

QMAKE_RPATHDIR += /usr/share/$${TARGET}/lib


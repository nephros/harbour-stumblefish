# SPDX-License-Identifier: MIT
TARGET = harbour-stumblefish-companion

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
    main.cpp \
    stumblefishcompanionclient.cpp

HEADERS += \
    stumblefishcompanionclient.h \
    $${STUMBLEFISH_ROOT_DIR}/common/constants.h

#HEADERS += \
#    $${STUMBLEFISH_ROOT_DIR}/src/stumblefishclient.h
#SOURCES += \
#    $${STUMBLEFISH_ROOT_DIR}/src/stumblefishclient.cpp

DISTFILES += \
    harbour-stumblefish-companion.desktop
#    Stumblefish.permission \
#    harbour-stumblefish.desktop \
#    icons/86x86/apps/harbour-stumblefish.png \
#    icons/108x108/apps/harbour-stumblefish.png \
#    icons/128x128/apps/harbour-stumblefish.png \
#    icons/172x172/apps/harbour-stumblefish.png \
#    icons/motorcycle-fish-forward.png \
#    icons/motorcycle-fish.png \
#    qml/harbour-stumblefish.qml \
#    qml/cover/CoverPage.qml \
#    qml/pages/AboutPage.qml \
#    qml/pages/MainPage.qml \
#    qml/pages/MapPage.qml \
#    qml/pages/ReportDetailPage.qml \
#    qml/pages/ReportsPage.qml \
#    qml/pages/SettingsPage.qml

#INSTALLS += target qml desktop sailjail_permission about_image
INSTALLS += target

target.path = /usr/bin

qml.files = qml
qml.path = /usr/share/$${TARGET}

# sailjail_permission.files = Stumblefish.permission
# sailjail_permission.path = /etc/sailjail/permissions

# about_image.files = icons/motorcycle-fish.png icons/motorcycle-fish-forward.png
# about_image.path = /usr/share/$${TARGET}/icons

QMAKE_RPATHDIR += /usr/share/$${TARGET}/lib

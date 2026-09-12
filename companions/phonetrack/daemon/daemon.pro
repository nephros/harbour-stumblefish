# SPDX-License-Identifier: MIT
TARGET = harbour-trackfishd

CONFIG += console c++11 link_pkgconfig
QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

QT -= gui
#QT += core dbus network positioning
QT += core dbus network

# PKGCONFIG += connman-qt5 nemonotifications-qt5 qofonoext systemsettings

INCLUDEPATH += . ../../../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += TRACK_MY_PHONE

TARGET.depends += harbour-stumblefishd

GLOBAL_COMMON_DIR = ../../../common
COMPANIONS_COMMON_DIR = ../../common

SOURCES += \
    main.cpp \
    settings.cpp \
    trackuploader.cpp \
    phonetrackdaemon.cpp \
    $${GLOBAL_COMMON_DIR}/phonetrackconfig.cpp

HEADERS += \
    settings.h \
    trackuploader.h \
    phonetrackdaemon.h \
    $${GLOBAL_COMMON_DIR}/phonetrackconfig.h

INSTALLS += target service dbusservice

target.path = /usr/bin

service.files = harbour-trackfishd.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.Tracker.service
dbusservice.path = /usr/share/dbus-1/services

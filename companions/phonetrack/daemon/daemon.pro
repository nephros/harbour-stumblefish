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

INCLUDEPATH += . .. ../..
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += TRACK_MY_PHONE

TARGET.depends += harbour-stumblefishd

include(../../companions.pri)

SOURCES += \
    main.cpp \
    trackuploader.cpp \
    phonetrackdaemon.cpp \
    ../config/phonetrackconfig.cpp

HEADERS += \
    trackuploader.h \
    phonetrackdaemon.h \
    ../config/phonetrackconfig.h

INSTALLS += target service dbusservice

target.path = /usr/libexec

service.files = harbour-trackfishd.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.Companion.Tracker.service
dbusservice.path = /usr/share/dbus-1/services

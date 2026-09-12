# SPDX-License-Identifier: MIT
TARGET = harbour-trackfishd

CONFIG += console c++11 link_pkgconfig
QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

QT -= gui
#QT += core dbus network positioning
QT += core dbus

# PKGCONFIG += connman-qt5 nemonotifications-qt5 qofonoext systemsettings

INCLUDEPATH += . ../../common ../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += TRACK_MY_PHONE

SOURCES += \
    main.cpp \
    settings.cpp \
    trackuploader.cpp \
    phonetrackdaemon.cpp \
    ../../common/phonetrackconfig.cpp

HEADERS += \
    settings.h \
    trackuploader.h \
    phonetrackdaemon.h \
    ../../common/phonetrackconfig.h

INSTALLS += target service dbusservice

target.path = /usr/bin

service.files = harbour-trackfishd.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.Tracker.service
dbusservice.path = /usr/share/dbus-1/services

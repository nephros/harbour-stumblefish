# SPDX-License-Identifier: MIT
TEMPLATE = app
TARGET = harbour-glassfishd

CONFIG += console c++11 link_pkgconfig

INCLUDEPATH += . ../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += FIND_KLABAUTERS

SOURCES += main.cpp \
           glassfish.cpp
HEADERS += glassfish.h

TARGET.depends += harbour-stumblefishd

include(../companions.pri)

INSTALLS += target service dbusservice

target.path = /usr/bin

service.files = harbour-glassfishd.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.Lookout.service
dbusservice.path = /usr/share/dbus-1/services


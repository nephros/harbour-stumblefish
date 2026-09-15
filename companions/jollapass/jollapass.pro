# SPDX-License-Identifier: MIT
TEMPLATE = app
TARGET = harbour-jollapassd

CONFIG += console c++11 link_pkgconfig

INCLUDEPATH += . ../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += FIND_JOLLA_BUDDIES

SOURCES += main.cpp \
           jollapass.cpp
HEADERS += jollapass.h

TARGET.depends += harbour-stumblefishd

include(../companions.pri)

INSTALLS += target service dbusservice

target.path = /usr/bin

service.files = harbour-jollapassd.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.JollaPass.service
dbusservice.path = /usr/share/dbus-1/services


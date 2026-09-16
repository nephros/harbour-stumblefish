# SPDX-License-Identifier: MIT
TEMPLATE = app
TARGET = harbour-passfishd

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

service.files = harbour-passfishd.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.JollaPass.service
dbusservice.path = /usr/share/dbus-1/services


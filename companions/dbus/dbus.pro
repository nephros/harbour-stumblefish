# SPDX-License-Identifier: MIT
TEMPLATE = app
TARGET = harbour-stumblecompaniond

CONFIG += console c++11 link_pkgconfig

INCLUDEPATH += . ../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp \
           companionproxy.cpp
HEADERS += companionproxy.h

TARGET.depends += harbour-stumblefishd

include(../companions.pri)

INSTALLS += target service dbusservice dbusinterface

target.path = /usr/bin

service.files = $${TARGET}.service
service.path = /usr/lib/systemd/user

dbusservice.files = org.stumblefish.Companions.service
dbusservice.path = /usr/share/dbus-1/services

dbusinterface.files = org.stumblefish.Companions.xml
dbusinterface.path = /usr/share/dbus-1/interfaces/


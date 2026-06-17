# SPDX-License-Identifier: MIT
TARGET = harbour-glassfishd

CONFIG += cmdline
QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

QT -= gui
QT += dbus

INCLUDEPATH += . ../common

SOURCES += \
    main.cpp \
    glassfish.cpp
HEADERS += \
    glassfish.h

INSTALLS += target service

target.path = /usr/bin

service.files = harbour-glassfishd.service
service.path = /usr/lib/systemd/user

# SPDX-License-Identifier: MIT
TARGET = harbour-glassfishd

CONFIG += cmdline c++11
QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

QT -= gui
QT += dbus

CONFIG += warn_on

INCLUDEPATH += . ../common
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    main.cpp \
    glassfish.cpp
HEADERS += \
    glassfish.h

RESOURCES += \
    fingerprints.qrc

INSTALLS += target service

target.path = /usr/bin

service.files = harbour-glassfishd.service
service.path = /usr/lib/systemd/user

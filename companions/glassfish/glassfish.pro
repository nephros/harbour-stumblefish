# SPDX-License-Identifier: MIT
TEMPLATE = aux
TARGET = harbour-glassfishd

CONFIG += console c++11 link_pkgconfig
QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

QT -= gui
#QT += core dbus network positioning
QT += core dbus

INCLUDEPATH += . ../../../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += GLASSFISH

TARGET.depends += harbour-stumblefishd

include(../../common/common.pri)



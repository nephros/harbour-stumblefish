# SPDX-License-Identifier: MIT
TEMPLATE = app
TARGET = harbour-phonetrackd

CONFIG += console c++11 link_pkgconfig

INCLUDEPATH += . ../
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += TRACK_MY_PHONE

SOURCES += main.cpp \
           phonetrack.cpp
HEADERS += phonetrack.h

TARGET.depends += harbour-stumblefishd

include(../companions.pri)

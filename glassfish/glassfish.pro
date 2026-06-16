TARGET = harbour-glassfishd

QT -= gui
QT += dbus
CONFIG += cmdline

SOURCES += src/main.cpp \
           src/glassfish.cpp
HEADERS += src/glassfish.h

unit.files += systemd/$$TARGET.service
unit.path = /usr/lib/systemd/user
INSTALLS += unit

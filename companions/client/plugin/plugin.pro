TEMPLATE = lib
TARGET = stumblecompanions
TARGET = $$qtLibraryTarget($$TARGET)
CONFIG += qt plugin qmltypes

QT += qml
QT -= gui dbus network

QML_IMPORT_NAME = org.stumblefish.companions
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_PATH = /usr/share/harbour-stumblefish/lib

message("Building the client QML plugin")

SOURCES += plugin.cpp

qmldir.files += qmldir
qmldir.path = /usr/share/harbour-stumblefish/lib
target.path = /usr/share/harbour-stumblefish/lib
INSTALLS += qmldir target

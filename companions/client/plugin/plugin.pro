TEMPLATE = lib
TARGET = trackfishqml

message("Building the client QML plugin")

CONFIG += qt plugin
QT += qml quick

SOURCES += plugin.cpp

INSTALLS += target
target.path = /usr/share/harbour-stumblefish/lib

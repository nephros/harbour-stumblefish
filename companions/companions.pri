
COMPANIONS_BASE_DIR = $$PWD/base
STUMBLEFISH_ROOT_DIR = $$PWD/..

QT -= gui
QT += core dbus

QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

INCLUDEPATH += \
               $${STUMBLEFISH_ROOT_DIR} \
               $${COMPANIONS_BASE_DIR}

HEADERS += \
    $${COMPANIONS_BASE_DIR}/companionbase.h

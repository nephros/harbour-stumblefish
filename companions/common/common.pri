GLOBAL_COMMON_DIR = $$PWD/../../common
COMPANIONS_COMMON_DIR = $$PWD

INCLUDEPATH += \
            $${COMPANIONS_COMMON_DIR} \
            $${GLOBAL_COMMON_DIR}

HEADERS += \
    versions.h \
    constants.h \
    abstractcompanion.h

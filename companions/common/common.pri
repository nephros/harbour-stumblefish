GLOBAL_COMMON_DIR = ../../common
COMPANIONS_COMMON_DIR = $PWD

INCLUDEPATHS += \
            $${COMPANIONS_COMMON_DIR} \
            $${GLOBAL_COMMON_DIR}

HEADERS += \
    versions.h \
    constants.h \
    abstractcompanion.h

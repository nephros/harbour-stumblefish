TEMPLATE = aux

CONFIG += sailfishapp_qml
CONFIG += sailfishapp_no_deploy_qml
#CONFIG += sailfishapp_i18n
CONFIG += no_link

QT -= gui

lupdate_only {
    SOURCES += $$files(*.qml)
}

#TRANSLATIONS += $$files(translations/*.ts)

qmlfiles.files += $$files(*.qml)
qmlfiles.path += /usr/share/harbour-stumblefish/qml/pages/components/
INSTALLS += qmlfiles

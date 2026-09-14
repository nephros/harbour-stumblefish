TEMPLATE = subdirs

SUBDIRS += ./daemon

phonetrackini.path = /usr/share/harbour-stumblefish/lib/phonetrack/
phonetrackini.files += config/phonetrack.ini
INSTALLS += phonetrackini

DISTFILES += companions/phonetrack/config/phonetrack.ini

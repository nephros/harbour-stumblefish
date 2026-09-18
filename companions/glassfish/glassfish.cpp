// SPDX-License-Identifier: MIT

#include "glassfish.h"
#include <QObject>
#include <QDebug>

//Companion::~Companion(){};

Companion::Companion(QObject *parent)
        : StumblefishCompanionBase(parent)
{
    Q_UNUSED(parent);
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(QString::fromLatin1(Glassfish::ServiceName))) {
        qWarning() << "Failed to register D-Bus service" << bus.lastError().message();
    } else
        qInfo() << "Registered D-Bus service" << QString::fromLatin1(Glassfish::ServiceName);

    if (!bus.registerObject(QString::fromLatin1(Glassfish::ObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << bus.lastError().message();
    } else
        qInfo() << "Registered D-Bus object" << QString::fromLatin1(Glassfish::ObjectPath);
};

void Companion::handleDBusMethod()
{
    qDebug() << Q_FUNC_INFO;
    // Implementation
}

// SPDX-License-Identifier: MIT

#include "glassfish.h"
#include <QObject>
#include <QDebug>

//Companion::~Companion(){};

Companion::Companion(QObject *parent)
        : StumblefishCompanionBase(parent)
{
    Q_UNUSED(parent);
    QDBusConnection service = QDBusConnection::connectToBus(QDBusConnection::SessionBus, QString::fromLatin1(Glassfish::ServiceName));
    if(service.isConnected())
        qInfo() << "Connected to D-Bus service" << QString::fromLatin1(Glassfish::ServiceName);
    if (!service.registerObject(QString::fromLatin1(Glassfish::ObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << service.lastError().message();
    } else
        qInfo() << "Registerred D-Bus object" << QString::fromLatin1(Glassfish::ObjectPath);
};

void Companion::handleDBusMethod()
{
    qDebug() << Q_FUNC_INFO;
    // Implementation
}

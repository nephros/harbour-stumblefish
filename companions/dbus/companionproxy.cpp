// SPDX-License-Identifier: MIT

#include "companionproxy.h"
#include <QObject>
#include <QDebug>

//CompanionProxy::~CompanionProxy(){};
CompanionProxy::CompanionProxy(QObject *parent)
    : QObject(parent)
    , m_companions(QStringList())
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(QString::fromLatin1(Stumblefish::CompanionServiceName))) {
        qCritical() << "Failed to register D-Bus service" << bus.lastError().message();
        CouldNotStartException *e = new CouldNotStartException();
        e->setMessage(QString("Fatal error: %1 (%2)").arg(bus.lastError().message()).arg((int)bus.lastError().type()));
        e->setCode((int)bus.lastError().type());
        throw e;
    } else
        qInfo() << "Registered D-Bus service" << QString::fromLatin1(Stumblefish::CompanionServiceName);

    if (!bus.registerObject(QString::fromLatin1(Stumblefish::CompanionObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << bus.lastError().message();
    } else
        qInfo() << "Registerred D-Bus object" << QString::fromLatin1(Stumblefish::CompanionObjectPath);
};

void CompanionProxy::handleDBusMethod()
{
    qDebug() << Q_FUNC_INFO;
    // Implementation
}
void CompanionProxy::registerCompanion(const QString& name)
{
    if(!m_companions.contains(name)) {
        m_companions.append(name);
        qDebug() << "CompanionProxy: registered" << name;
    }
}


QStringList CompanionProxy::availableCompanions()
{
    QStringList names;
    names
        << QString::fromLatin1(Trackfish::ApplicationName)
        << QString::fromLatin1(Jollapass::ApplicationName)
        << QString::fromLatin1(Glassfish::ApplicationName);

    for (const QString& companion : names) {
        QDBusInterface *iface = ifaceFor(companion);
        if(iface && iface->isValid()) {
            registerCompanion(companion);
            iface->deleteLater();
        } else {
            qDebug() << "CompanionProxy: unlisting Companion:" << companion;
            m_companions.removeAt(m_companions.indexOf(companion));
        }
    }

    qDebug() << "CompanionProxy: listing Companions:" << m_companions.join(",");
    return m_companions;
}


QDBusInterface* CompanionProxy::ifaceFor(const QString& companion)
{
    QDBusInterface *iface = nullptr;
    if(m_companions.contains(companion)) {
        if(companion == (QString::fromLatin1(Trackfish::ApplicationName))) {
            iface = new QDBusInterface(QString::fromLatin1(Trackfish::ServiceName),
                                       QString::fromLatin1(Trackfish::ObjectPath),
                                       QString::fromLatin1(Trackfish::InterfaceName));
        } else if(companion == (QString::fromLatin1(Glassfish::ApplicationName))) {
            iface = new QDBusInterface(QString::fromLatin1(Glassfish::ServiceName),
                                       QString::fromLatin1(Glassfish::ObjectPath),
                                       QString::fromLatin1(Glassfish::InterfaceName));
        } else if(companion == (QString::fromLatin1(Jollapass::ApplicationName))) {
            iface = new QDBusInterface(QString::fromLatin1(Jollapass::ServiceName),
                                       QString::fromLatin1(Jollapass::ObjectPath),
                                       QString::fromLatin1(Jollapass::InterfaceName));
        } else {
            qWarning() << "No interface for:" << companion;
        }
    }
    if(!iface)
        qWarning() << "NULL interface for:" << companion;
    if(!iface->isValid())
        qWarning() << "Invalid interface for:" << companion;
    return iface;
}

// SPDX-License-Identifier: MIT
#include "stumblefishcompanionclient.h"

#include "common/constants.h"
#include "base/constants.h"

#include <QDBusConnection>
//#include <QDBusArgument>
#include <QDBusInterface>
//#include <QDBusMessage>
//#include <QDBusPendingCall>
//#include <QDBusPendingCallWatcher>
//#include <QDBusPendingReply>
#include <QDBusReply>
//#include <QDBusVariant>
//#include <QtGlobal>
#include <QDebug>




StumblefishCompanionClient::StumblefishCompanionClient(QObject *parent)
    : QObject(parent)
    , m_interface(new QDBusInterface(QString::fromLatin1(Stumblefish::ServiceName),
                                     QString::fromLatin1(Stumblefish::ObjectPath),
                                     QString::fromLatin1(Stumblefish::InterfaceName),
                                     QDBusConnection::sessionBus(),
                                     this))
    , m_companions(QStringList())
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.connect(QString::fromLatin1(Stumblefish::ServiceName),
                QString::fromLatin1(Stumblefish::ObjectPath),
                QString::fromLatin1(Stumblefish::InterfaceName),
                QStringLiteral("statusChanged"),
                this,
                SLOT(handleStatusSignal(QVariantMap)));
    bus.connect(QString::fromLatin1(Stumblefish::ServiceName),
                QString::fromLatin1(Stumblefish::ObjectPath),
                QString::fromLatin1(Stumblefish::InterfaceName),
                QStringLiteral("settingsChanged"),
                this,
                SLOT(handleSettingsSignal(QVariantMap)));
}

//StumblefishCompanionClient::~StumblefishCompanionClient()
//{
//}


QVariantMap StumblefishCompanionClient::status() const
{
    return m_status;
}

QVariantMap StumblefishCompanionClient::settings() const
{
    return m_settings;
}

void StumblefishCompanionClient::handleStatusSignal(const QVariantMap &status)
{
    m_status = status;
}
void StumblefishCompanionClient::handleSettingsSignal(const QVariantMap &settings)
{
    m_settings = settings;
}

void StumblefishCompanionClient::registerCompanion(const QString& name)
{
    if(!m_companions.contains(name))
        m_companions.append(name);
}

QStringList StumblefishCompanionClient::availableCompanions()
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
        } else
            m_companions.removeAt(m_companions.indexOf(companion));
    }

    return m_companions;
}

QVariantMap StumblefishCompanionClient::companionSettings(const QString& companion)
{
    QVariantMap map;
    QDBusInterface *iface = ifaceFor(companion);
    if(iface && iface->isValid()) {
        QDBusReply<QVariantMap> reply = iface->call("settings");
        if(reply.isValid())
            map = reply.value();
        iface->deleteLater();
    }
    return map;
}

void StumblefishCompanionClient::setCompanionSettings(const QString& companion, const QString& key, const QVariant& value)
{
    QDBusInterface *iface = ifaceFor(companion);
    if(iface->isValid()) {
        iface->call("setSetting", key, value);
        iface->deleteLater();
    }
}

// FIXME: Magic strings unnecessary
QDBusInterface* StumblefishCompanionClient::ifaceFor(const QString& companion)
{
    QDBusInterface *iface = nullptr;
    if(m_companions.contains(companion)) {
        if(companion == (QString::fromLatin1(Trackfish::ApplicationName))) {
            iface = new QDBusInterface("org.stumblefish.Companions", "/org/stumblefish/Tracker", "org.stumblefish.Tracker");
        } else if(companion == (QString::fromLatin1(Glassfish::ApplicationName))) {
            iface = new QDBusInterface("org.stumblefish.Companions", "/org/stumblefish/Lookout", "org.stumblefish.Lookout");
        } else if(companion == (QString::fromLatin1(Jollapass::ApplicationName))) {
            iface = new QDBusInterface("org.stumblefish.Companions", "/org/stumblefish/JollaPass", "org.stumblefish.JollaPass");
        }
    }
    return iface;
}

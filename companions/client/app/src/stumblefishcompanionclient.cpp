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

// FIXME: Magic strings unnecessary
QStringList StumblefishCompanionClient::availableCompanions()
{
#ifdef TRACK_MY_PHONE
    if(QDBusInterface("org.stumblefish.Companions", "/org/stumblefish/Tracker", "org.stumblefish.Tracker").isValid()) {
        registerCompanion(QString::fromLatin1(Trackfish::ApplicationName));
    } else
        m_companions.removeAt(m_companions.indexOf(QString::fromLatin1(Trackfish::ApplicationName)));
#endif
#ifdef FIND_KLABAUTERS
    if(QDBusInterface("org.stumblefish.Companions", "/org/stumblefish/JollaPass", "org.stumblefish.JollaPass").isValid()) {
        registerCompanion(QString::fromLatin1(Jollapass::ApplicationName));
    } else
        m_companions.removeAt(m_companions.indexOf(QString::fromLatin1(Jollapass::ApplicationName)));
#endif
#ifdef FIND_JOLLA_BUDDIES
    if(QDBusInterface("org.stumblefish.Companions", "/org/stumblefish/Lookout", "org.stumblefish.Lookout").isValid()) {
        registerCompanion(QString::fromLatin1(Glassfish::ApplicationName));
    } else
        m_companions.removeAt(m_companions.indexOf(QString::fromLatin1(Glassfish::ApplicationName)));
#endif
    return m_companions;
}

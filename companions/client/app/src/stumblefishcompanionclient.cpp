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
    qDebug() << "StumblefishCompanionClient created.";
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

QDBusInterface* StumblefishCompanionClient::ifaceFor(const QString& companion)
{
    QDBusInterface *iface = nullptr;
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
    if(!iface)
        qWarning() << "NULL interface for:" << companion;
    if(!iface->isValid())
        qWarning() << "Invalid interface for:" << companion;
    return iface;
}

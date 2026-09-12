// SPDX-License-Identifier: MIT
#include "phonetrackdaemon.h"

#include "constants.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDBusConnection>
#include <QDebug>
//#include <QGeoCoordinate>
//#include <QProcess>
//#include <QSet>
#include <QStringList>
//#include <QTimer>

#include "settings.h"
#include <climits>

namespace {
const char PhoneTrackEnableKey[] = "phonetrack/enable";
const char PhoneTrackLiveKey[] = "phonetrack/liveMode";
const char PhoneTrackTypeKey[] = "phonetrack/type";
const char PhoneTrackUrlKey[] = "phonetrack/url";
const char PhoneTrackSessionKey[] = "phonetrack/session";
const char PhoneTrackNameKey[] = "phonetrack/name";

static qint64 _phoneTrackSubmissions = 0;
static qint64 _phoneTrackSubmissionsSkipped = 0;
static qint64 _phoneTrackLastSubmission = 0;
const int _phoneTrackMinSubmissionInterval = 1000 * 60 * 15;

}

Watcher::Watcher(QObject *parent)
    : QObject(parent)
    , m_phoneTrackConfig(new Stumblefish::PhoneTrackConfig())
    , m_daemonWatcher(this)
    , m_interface(new QDBusInterface(QString::fromLatin1(Stumblefish::ServiceName),
                            QString::fromLatin1(Stumblefish::ObjectPath),
                            QString::fromLatin1(Stumblefish::InterfaceName),
                            QDBusConnection::sessionBus(),
                            this))
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    m_daemonWatcher.setConnection(bus);
    //m_daemonWatcher.setWatchMode(QDBusWatcherWatcher::WatchForUnregistration);
    connect(&m_daemonWatcher, SIGNAL(settingsChanged),
             this, SLOT(onSettingsChanged(QVariant)));
    connect(&m_daemonWatcher, SIGNAL(sstatusChanged),
             this, SLOT(onsStatusChanged(QVariant)));
    connect(&m_daemonWatcher, SIGNAL(reportsChanged),
             this, SLOT(onReportsChanged()));
/*
    //if (!bus.registerWatcher(QString::fromLatin1(Trackfish::WatcherName))) {
    if (!bus.registerWatcher(QString::fromLatin1(Stumblefish::WatcherName))) {
        qWarning() << "Failed to register D-Bus service" << bus.lastError().message();
    }
    if (!bus.registerObject(QString::fromLatin1(Trackfish::ObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << bus.lastError().message();
    }
*/
}

Watcher::~Watcher()
{
}

QVariantMap Watcher::settings() const
{
}

QVariantMap Watcher::status() const
{
    QVariantMap map;
    map.insert(QStringLiteral("phoneTrackEnabled"), m_settings.phoneTrackEnabled());
    map.insert(QStringLiteral("phoneTrackLiveMode"), m_settings.phoneTrackLiveMode());
    map.insert(QStringLiteral("phoneTrackSubmissions"), QVariant::fromValue(_phoneTrackSubmissions));
    map.insert(QStringLiteral("phoneTrackSubmissionsSkipped"), QVariant::fromValue(_phoneTrackSubmissionsSkipped));
    map.insert(QStringLiteral("canApplyLiveTrackConfig"), m_settings.phoneTrackEnabled());
    return map;
}

void Watcher::onReportsChanged()
{
    if (m_settings.phoneTrackEnabled() && m_settings.phoneTrackLiveMode()) {
        if ((QDateTime::currentMSecsSinceEpoch() - _phoneTrackLastSubmission) > _phoneTrackMinSubmissionInterval) {
            // lets  e a bit more accurate here
            if (fix.accuracy > 0.0 && fix.accuracy < 50.0) {
                report.position.satellites = m_position.satellitesInUse();
                report.battery =  m_battery.chargePercentage();
                m_uploader.uploadTracked(report);
                _phoneTrackLastSubmission = QDateTime::currentMSecsSinceEpoch();
                _phoneTrackSubmissions++;
            } else
                _phoneTrackSubmissionsSkipped++;
        } else
            qInfo() << "PhoneTrack: skipped sumbission, too soon";
    }
}
void Watcher::onSettingsChanged(QVariant)
{
}
void Watcher::onStatusChanged(QVariant)
{
}

void Watcher::asyncCall(const QString &method, const QVariantList &arguments, const QString &kind)
{
        QDBusPendingCall call = m_interface->asyncCallWithArgumentList(method, arguments);
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        watcher->setProperty("kind", kind);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                        this, SLOT(pendingFinished(QDBusPendingCallWatcher*)));
        //setBusyCount(m_busyCount + 1);
}
void Watcher::setSetting(const QString &key, const QVariant &value)
{
        QVariantList arguments;
        arguments << key << QVariant::fromValue(QDBusVariant(value));
        asyncCall(QStringLiteral("setSetting"), arguments, QStringLiteral("void"));
}
bool Watcher::canApplyLiveTrackConfig() {
    return m_phoneTrackConfig->haveLiveTrackConfig() ;
//        && m_settings.phoneTrackEnabled();
}
void Watcher::applyLiveTrackConfig()
{
    setSetting(QString::fromLatin1(PhoneTrackUrlKey),
               m_phoneTrackConfig->liveTrackConfig()->value("UrlTemplate").toString());
    setSetting(QString::fromLatin1(PhoneTrackSessionKey),
               m_phoneTrackConfig->liveTrackConfig()->value("SessionID").toString());
    setSetting(QString::fromLatin1(PhoneTrackNameKey),
               m_phoneTrackConfig->liveTrackConfig()->value("DeviceID").toString());
}


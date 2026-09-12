// SPDX-License-Identifier: MIT

#include <QCoreApplication>
#include <QDateTime>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QDebug>
#include <QStringList>

#include "common/constants.h"
#include "phonetrackdaemon.h"
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

const char StumblefishReportsMethod[] = "reports";
}

Companion::Companion(QObject *parent)
    : QObject(parent)
    , m_stumbleService(new QDBusInterface(QString::fromLatin1(Stumblefish::ServiceName),
                                     QString::fromLatin1(Stumblefish::ObjectPath),
                                     QString::fromLatin1(Stumblefish::InterfaceName),
                                     QDBusConnection::sessionBus(),
                                     this))

    , m_stumbleWatcher(this)
    , m_phoneTrackConfig(new Stumblefish::PhoneTrackConfig())
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    m_stumbleWatcher.setConnection(bus);

    if (!bus.registerService(QString::fromLatin1(Stumblefish::ServiceName))) {
        qWarning() << "Failed to register D-Bus service" << bus.lastError().message();
    }
    if (!bus.registerObject(QString::fromLatin1(Trackfish::ObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << bus.lastError().message();
    }
    //stumblefish.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    QObject::connect(&m_stumbleWatcher, SIGNAL(settingsChanged),
             this, SLOT(onSettingsChanged(QVariant)));
    QObject::connect(&m_stumbleWatcher, SIGNAL(sstatusChanged),
             this, SLOT(onsStatusChanged(QVariant)));
    QObject::connect(&m_stumbleWatcher, SIGNAL(reportsChanged),
             this, SLOT(onReportsChanged()));


}

Companion::~Companion()
{
}

QVariantMap Companion::settings() const
{
    QVariantMap map;
    return map;
}

QVariantMap Companion::status() const
{
    QVariantMap map;
    /*
    map.insert(QStringLiteral("phoneTrackEnabled"), m_settings.phoneTrackEnabled());
    map.insert(QStringLiteral("phoneTrackLiveMode"), m_settings.phoneTrackLiveMode());
    map.insert(QStringLiteral("phoneTrackSubmissions"), QVariant::fromValue(_phoneTrackSubmissions));
    map.insert(QStringLiteral("phoneTrackSubmissionsSkipped"), QVariant::fromValue(_phoneTrackSubmissionsSkipped));
    map.insert(QStringLiteral("canApplyLiveTrackConfig"), m_settings.phoneTrackEnabled());
    */
    return map;
}

void Companion::onReportsChanged()
{
    QVariantList arguments;
    arguments << QVariant::fromValue(960);

    QDBusPendingCall call = m_stumbleService->asyncCall(QString::fromLatin1(StumblefishReportsMethod));
    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, &m_uploader, &TrackUploader::reportsHandler);

    /*
    Report report;
    if (m_settings.phoneTrackEnabled() && m_settings.phoneTrackLiveMode()) {
        if ((QDateTime::currentMSecsSinceEpoch() - _phoneTrackLastSubmission) > _phoneTrackMinSubmissionInterval) {

            // lets  e a bit more accurate here
            if (fix.accuracy > 0.0 && fix.accuracy < 50.0) {
                report.position.satellites = m_report.position.satellites();
                report.battery =  m_battery.chargePercentage();
                m_uploader.uploadTracked(report);
                _phoneTrackLastSubmission = QDateTime::currentMSecsSinceEpoch();
                _phoneTrackSubmissions++;
            } else
                _phoneTrackSubmissionsSkipped++;

        } else
            qInfo() << "PhoneTrack: skipped sumbission, too soon";
    }
    */
}
void Companion::onSettingsChanged(QVariant)
{
}
void Companion::onStatusChanged(QVariant)
{
}

/*
void Companion::asyncCall(const QString &method, const QVariantList &arguments, const QString &kind)
{
        QDBusPendingCall call = m_interface->asyncCallWithArgumentList(method, arguments);
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        watcher->setProperty("kind", kind);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                        this, SLOT(pendingFinished(QDBusPendingCallWatcher*)));
        //setBusyCount(m_busyCount + 1);
}
void Companion::setSetting(const QString &key, const QVariant &value)
{
        QVariantList arguments;
        arguments << key << QVariant::fromValue(QDBusVariant(value));
        asyncCall(QStringLiteral("setSetting"), arguments, QStringLiteral("void"));
}
void Companion::getReport(int reportId)
{
        QVariantList arguments;
        arguments << QVariant::fromValue(QDBusVariant(reportId));
        asyncCall(QStringLiteral("report"), arguments, QStringLiteral("void"));
}
*/

bool Companion::canApplyLiveTrackConfig() {
    return m_phoneTrackConfig->haveLiveTrackConfig()
        && m_settings.phoneTrackEnabled();
}
void Companion::applyLiveTrackConfig()
{
    m_settings.setValue(QString::fromLatin1(PhoneTrackUrlKey),
               m_phoneTrackConfig->liveTrackConfig()->value("UrlTemplate").toString());
    m_settings.setValue(QString::fromLatin1(PhoneTrackSessionKey),
               m_phoneTrackConfig->liveTrackConfig()->value("SessionID").toString());
    m_settings.setValue(QString::fromLatin1(PhoneTrackNameKey),
               m_phoneTrackConfig->liveTrackConfig()->value("DeviceID").toString());
}

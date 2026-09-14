// SPDX-License-Identifier: MIT

#include <QCoreApplication>
#include <QDateTime>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QStringList>
#include <QDebug>

#include "common/constants.h"
#include "config/phonetrackconfig.h"
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

//static qint64 _phoneTrackSubmissions = 0;
//static qint64 _phoneTrackSubmissionsSkipped = 0;
//static qint64 _phoneTrackLastSubmission = 0;
//const int _phoneTrackMinSubmissionInterval = 1000 * 60 * 15;

const char StumblefishReportsMethod[] = "reports";
}

Companion::Companion(QObject *parent)
    : StumblefishCompanionBase(parent)
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
    m_stumbleWatcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    m_stumbleWatcher.addWatchedService((Stumblefish::ServiceName));
    QObject::connect(&m_stumbleWatcher, SIGNAL(serviceUnregistered(const QString&)),
                            this, SLOT(onStumblefishVanished(const QString&)));


    if (!bus.registerService(QString::fromLatin1(Trackfish::ServiceName))) {
        qWarning() << "Failed to register D-Bus service" << bus.lastError().message();
    }
    if (!bus.registerObject(QString::fromLatin1(Trackfish::ObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << bus.lastError().message();
    }

    if(!bus.connect(QString::fromLatin1(Stumblefish::ServiceName),
                    QString::fromLatin1(Stumblefish::ObjectPath),
                    QString::fromLatin1(Stumblefish::InterfaceName),
                    QStringLiteral("settingsChanged"),
                    this, SLOT(onSettingsChanged(QVariantMap))) ) {
        qWarning() << "Failed to connect D-Bus signal settingsChanged" << bus.lastError().message();
     } else {
        qDebug() << "Watching D-Bus signal" << QString::fromLatin1(Stumblefish::ServiceName)
                        << QString::fromLatin1(Stumblefish::ObjectPath)
                        << QString::fromLatin1(Stumblefish::InterfaceName)
                        << QStringLiteral("settingsChanged");
    }

    if(!bus.connect(QString::fromLatin1(Stumblefish::ServiceName),
                    QString::fromLatin1(Stumblefish::ObjectPath),
                    QString::fromLatin1(Stumblefish::InterfaceName),
                    QStringLiteral("statusChanged"),
                    this, SLOT(onStatusChanged(QVariantMap))) ) {
        qWarning() << "Failed to connect D-Bus signal statusChanged" << bus.lastError().message();
    } else {
       qDebug() << "Watching D-Bus signal" << QString::fromLatin1(Stumblefish::ServiceName)
                       << QString::fromLatin1(Stumblefish::ObjectPath)
                       << QString::fromLatin1(Stumblefish::InterfaceName)
                       << QStringLiteral("statusChanged");
    }

    if(!bus.connect(QString::fromLatin1(Stumblefish::ServiceName),
                    QString::fromLatin1(Stumblefish::ObjectPath),
                    QString::fromLatin1(Stumblefish::InterfaceName),
                    QStringLiteral("reportsChanged"),
                    this, SLOT(onReportsChanged())) ) {
        qWarning() << "Failed to connect D-Bus signal reportsChanged" << bus.lastError().message();
    } else {
       qDebug() << "Watching D-Bus signal" << QString::fromLatin1(Stumblefish::ServiceName)
                       << QString::fromLatin1(Stumblefish::ObjectPath)
                       << QString::fromLatin1(Stumblefish::InterfaceName)
                       << QStringLiteral("reportsChanged");
    }
}

void Companion::handleDBusMethod()
{
    qDebug() << Q_FUNC_INFO;
}

void Companion::onStumblefishVanished(const QString& service)
{
    Q_UNUSED(service)
    qWarning() << "Stumblefish exited, quitting!";
    qApp->quit();
}

QVariantMap Companion::settings() const
{
    qDebug() << Q_FUNC_INFO;
    QVariantMap map;
    return map;
}

QVariantMap Companion::status() const
{
    qDebug() << Q_FUNC_INFO;
    QVariantMap map;
    /*
    map.insert(QStringLiteral("phoneTrackEnabled"), m_settings.phoneTrackEnabled());
    map.insert(QStringLiteral("phoneTrackLiveMode"), m_settings.phoneTrackLiveMode());
    map.insert(QStringLiteral("phoneTrackSubmissions"), QVariant::fromValue(_phoneTrackSubmissions));
    map.insert(QStringLiteral("phoneTrackSubmissionsSkipped"), QVariant::fromValue(_phoneTrackSubmissionsSkipped));
    map.insert(QStringLiteral("canApplyLiveTrackConfig"), m_settings.phoneTrackEnabled());
    */
    /*
    const PositionFix fix = m_position.lastFix();
    map.insert(QStringLiteral("direction"), fix.direction);
    map.insert(QStringLiteral("speed"), fix.speed);
    */
    return map;
}

void Companion::onReportsChanged()
{
    qDebug() << Q_FUNC_INFO;
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
void Companion::onSettingsChanged(const QVariantMap& settings)
{
}

void Companion::onStatusChanged(const QVariantMap& status)
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

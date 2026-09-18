// SPDX-License-Identifier: MIT

#include <QCoreApplication>
#include <QDateTime>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QStringList>
#include <QTimer>
#include <QDebug>

#include "common/constants.h"
#include "keys.h"
#include "phonetrackdaemon.h"

#include <climits>

namespace {
static qint64 _phoneTrackSubmissions = 0;
static qint64 _phoneTrackSubmissionsSkipped = 0;
//static qint64 _phoneTrackLastSubmission = 0;
const int _phoneTrackMinSubmissionInterval = 1000 * 15;
const int _phoneTrackMinAccuracy = 15;

const char StumblefishReportsMethod[] = "reports";
const char StumblefishCollectMethod[] = "collectNow";

static bool isWorthSubmitting(bool fix, bool gnss, double acc, qulonglong last)
{
    if (!fix || !gnss) return false;
    if (acc > _phoneTrackMinAccuracy) return false;
    qulonglong ts = QDateTime::currentMSecsSinceEpoch();
    if ((ts - last) < _phoneTrackMinSubmissionInterval) return false;
    return true;
}

}

Companion::Companion(QObject *parent)
    : StumblefishCompanionBase(parent)
    , m_stumbleService(new QDBusInterface(QString::fromLatin1(Stumblefish::ServiceName),
                                     QString::fromLatin1(Stumblefish::ObjectPath),
                                     QString::fromLatin1(Stumblefish::InterfaceName),
                                     QDBusConnection::sessionBus(),
                                     this))

    , m_stumbleWatcher(this)
    , m_settings(Trackfish::OrganizationName, Trackfish::ApplicationName)
    , m_phoneTrackConfig(new Stumblefish::PhoneTrackConfig())
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    /* watch for unregistration and quit if detected: */
    m_stumbleWatcher.setConnection(bus);
    m_stumbleWatcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    m_stumbleWatcher.addWatchedService((Stumblefish::ServiceName));
    QObject::connect(&m_stumbleWatcher, SIGNAL(serviceUnregistered(const QString&)),
                            this, SLOT(onStumblefishVanished(const QString&)));


    if (!bus.registerService(QString::fromLatin1(Trackfish::ServiceName))) {
        qWarning() << "Failed to register D-Bus service" << bus.lastError().message();
    } else
        qInfo() << "Registered D-Bus service" << QString::fromLatin1(Trackfish::ServiceName);

    if (!bus.registerObject(QString::fromLatin1(Trackfish::ObjectPath), this,
                            QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qWarning() << "Failed to register D-Bus object" << bus.lastError().message();
    } else
        qInfo() << "Registered D-Bus path" << QString::fromLatin1(Trackfish::ObjectPath);

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
//    QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(Stumblefish::ServiceName),
//                                                          QString::fromLatin1(Stumblefish::ObjectPath),
//                                                          QString::fromLatin1(Stumblefish::InterfaceName),
//                                                          QString::fromLatin1(StumblefishCollectMethod));
//    QTimer::singleShot(2000, QDBusConnection::sessionBus().send(message));

}

void Companion::handleDBusMethod()
{
    qDebug() << Q_FUNC_INFO;
}

void Companion::onStumblefishVanished(const QString& service)
{
    Q_UNUSED(service)
    if (service != Stumblefish::ServiceName)
        qWarning() << "Stumblefish exited, quitting" << service;
    else
        qWarning() << "Stumblefish exited, quitting!";
    qApp->quit();
}

QVariantMap Companion::settings() const
{
    QVariantMap map;
    for (const auto& key : m_settings.allKeys())
    {
        if ((key == QString::fromLatin1(PhoneTrackSessionKey))
        || (key == QString::fromLatin1(PhoneTrackNameKey))) {
            map.insert(key, "*****");
        } else
            map.insert(key, m_settings.value(key).toString());
    }
    return map;
}

QVariantMap Companion::status() const
{
//    qDebug() << Q_FUNC_INFO;
    QVariantMap map;
//    map.insert(QStringLiteral("phoneTrackEnabled"), phoneTrackEnabled());
//    map.insert(QStringLiteral("phoneTrackLiveMode"), phoneTrackLiveMode());
    map.insert(QStringLiteral("phoneTrackSubmissions"), QVariant::fromValue(_phoneTrackSubmissions));
    map.insert(QStringLiteral("phoneTrackSubmissionsSkipped"), QVariant::fromValue(_phoneTrackSubmissionsSkipped));
//    map.insert(QStringLiteral("canApplyLiveTrackConfig"), ());
    return map;
}

bool Companion::phoneTrackConfigValid()
{
    return !m_settings.value(QString::fromLatin1(PhoneTrackTypeKey)).toString().isEmpty()
        && !m_settings.value(QString::fromLatin1(PhoneTrackUrlKey)).toString().isEmpty()
        && !m_settings.value(QString::fromLatin1(PhoneTrackSessionKey)).toString().isEmpty();

}
void Companion::onReportsChanged()
{
    qDebug() << Q_FUNC_INFO;
    //QVariantList args;
    //args << QVariant::fromValue(960);
    QVariant args = QVariant::fromValue(960);

    QDBusPendingCall call = m_stumbleService->asyncCall(QString::fromLatin1(StumblefishReportsMethod), args);
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
    qDebug() << Q_FUNC_INFO;
    qDebug() << "Stumblefish settings::" << settings;
    if(settings.value(QString::fromLatin1(Stumblefish::BackgroundConfigKey)).toBool() == false)
        onStumblefishVanished(QString()); // quit
}

void Companion::onStatusChanged(const QVariantMap& status)
{
    qDebug() << Q_FUNC_INFO;
//    qDebug() << "Stumblefish status:" << status;
//    qDebug() << "    gnssBackedFix:" << status.value("gnssBackedFix").toString();
//    qDebug() << "    hasFix:" << status.value("hasFix").toString();

    if(!isWorthSubmitting(status.value("hasFix").toBool(),
                          status.value("gnssBackedFix").toBool(),
                          status.value("accuracy").toDouble(),
//                          status.value("fixTimestampMs").value<qulonglong>(),
                          m_lastReport)) {
//            qDebug() << "Skipping report.";
            _phoneTrackSubmissionsSkipped++;
            return;
    }
    m_lastReport = status.value("fixTimestampMs").value<qulonglong>();
    Trackfish::Report report;
    report.position.latitude    = status.value(QStringLiteral("latitude")).toDouble();
    report.position.longitude   = status.value(QStringLiteral("longitude")).toDouble();
    report.position.accuracy    = status.value(QStringLiteral("accuracy")).toDouble();
    report.position.direction   = status.value(QStringLiteral("direction")).toDouble();
    report.position.speed       = status.value(QStringLiteral("speed")).toDouble();
    report.position.satellites  = status.value(QStringLiteral("satellitesInUse")).toInt();
    qDebug() << "Report:"
             <<  status.value(QStringLiteral("latitude")).toDouble()
             <<  status.value(QStringLiteral("longitude")).toDouble()
             <<  status.value(QStringLiteral("accuracy")).toDouble()
             <<  status.value(QStringLiteral("direction")).toDouble()
             <<  status.value(QStringLiteral("speed")).toDouble()
             << status.value(QStringLiteral("satellitesInUse")).toInt();
    _phoneTrackSubmissions++;
    QVariantMap settings;
    settings.insert("phoneTrackType", m_settings.value(QString::fromLatin1(PhoneTrackTypeKey)).toString());
    settings.insert("phoneTrackUrlTemplate", m_settings.value(QString::fromLatin1(PhoneTrackUrlKey)).toString());
    settings.insert("phoneTrackSessionID",   m_settings.value(QString::fromLatin1(PhoneTrackSessionKey)).toString());
    settings.insert("phoneTrackDeviceID",    m_settings.value(QString::fromLatin1(PhoneTrackNameKey)).toString());
    m_uploader.uploadTracked(report, settings);
}

/*
void Companion::asyncCall(const QString &method, const QVariantList &arguments, const QString &kind)
{
        QDBusPendingCall call = m_stumbleService->asyncCallWithArgumentList(method, arguments);
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
        watcher->setProperty("kind", kind);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                        this, SLOT(pendingFinished(QDBusPendingCallWatcher*)));
        //setBusyCount(m_busyCount + 1);
}

void Companion::getReport(int reportId)
{
        QVariantList arguments;
        arguments << QVariant::fromValue(QDBusVariant(reportId));
        asyncCall(QStringLiteral("report"), arguments, QStringLiteral("void"));
}
*/

bool Companion::phoneTrackEnabled()
{
    return m_settings.value(QString::fromLatin1(PhoneTrackEnableKey)).toBool();
}

void Companion::setPhoneTrackEnabled(bool enable)
{
    m_settings.setValue(QString::fromLatin1(PhoneTrackEnableKey), QVariant::fromValue(enable));
}

bool Companion::phoneTrackLiveEnabled()
{
    return m_settings.value(QString::fromLatin1(PhoneTrackLiveKey)).toBool();
}

void Companion::setPhoneTrackLiveEnabled(bool enable)
{
    m_settings.setValue(QString::fromLatin1(PhoneTrackLiveKey), QVariant::fromValue(enable));
}

uint Companion::phoneTrackType()
{
    return m_settings.value(QString::fromLatin1(PhoneTrackTypeKey)).value<uint>();
}

void Companion::setPhoneTrackType(uint type)
{
    m_settings.setValue(QString::fromLatin1(PhoneTrackTypeKey), QVariant::fromValue(type));
}


void Companion::applyLiveTrackConfig()
{
}

bool Companion::canApplyLiveTrackConfig()
{
    return m_phoneTrackConfig->haveLiveTrackConfig();
}

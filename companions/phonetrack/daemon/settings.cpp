// SPDX-License-Identifier: MIT

#include "common/constants.h"
#include "companions/base/constants.h"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QTimer>

#include "settings.h"
#include "settings_keys.h"

namespace {
const char StumblefishSettingsMethod[] = "settings";
const char StumblefishSetValueMethod[] = "setSetting";
}

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_stumbleService(new QDBusInterface(QString::fromLatin1(Stumblefish::ServiceName),
                                     QString::fromLatin1(Stumblefish::ObjectPath),
                                     QString::fromLatin1(Stumblefish::InterfaceName),
                                     QDBusConnection::sessionBus(),
                                     this))
    , m_phoneTrackConfig()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
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
    updateSettings();
}

void Settings::updateSettings() {
    QDBusPendingCall call = m_stumbleService->asyncCall(QString::fromLatin1(StumblefishSettingsMethod), QVariant());
    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, SIGNAL(finished), this, SLOT(onSettingsUpdated()));

}

void Settings::onSettingsChanged(const QVariantMap& settings)
{
    m_settings = settings;
}

QVariantMap Settings::toMap() const
{
    return QVariantMap(m_settings);
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    QVariantList args;
    QVariantMap arg;
    arg.insert(key, value.toString());
    args << arg;
    QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(Stumblefish::ServiceName),
                                                          QString::fromLatin1(Stumblefish::ObjectPath),
                                                          QString::fromLatin1(Stumblefish::InterfaceName),
                                                          QString::fromLatin1(StumblefishSetValueMethod));
    message.setArguments(args);
    QDBusConnection::sessionBus().send(message);


}

void Settings::applyLiveTrackConfig()
{
    setValue(QString::fromLatin1(PhoneTrackUrlKey),
               m_phoneTrackConfig->liveTrackConfig()->value("UrlTemplate").toString());
    setValue(QString::fromLatin1(PhoneTrackSessionKey),
               m_phoneTrackConfig->liveTrackConfig()->value("SessionID").toString());
    setValue(QString::fromLatin1(PhoneTrackNameKey),
               m_phoneTrackConfig->liveTrackConfig()->value("DeviceID").toString());
}

bool Settings::canApplyLiveTrackConfig()
{
    return m_phoneTrackConfig->haveLiveTrackConfig();
}

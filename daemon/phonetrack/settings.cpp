// SPDX-License-Identifier: MIT
#include "settings.h"

#include "constants.h"

namespace {
const char PhoneTrackEnableKey[] = "phonetrack/enable";
const char PhoneTrackLiveKey[] = "phonetrack/liveMode";
const char PhoneTrackTypeKey[] = "phonetrack/type";
const char PhoneTrackUrlKey[] = "phonetrack/url";
const char PhoneTrackSessionKey[] = "phonetrack/session";
const char PhoneTrackNameKey[] = "phonetrack/name";
}

Settings::Settings(QObject *parent)
    : QObject(parent)
{
    ensureDefaults();
}

QVariantMap Settings::toMap() const
{
    QVariantMap map;

    map.insert(QStringLiteral("phoneTrackEnabled"),  phoneTrackEnabled());
    map.insert(QStringLiteral("phoneTrackLiveMode"),     phoneTrackLiveMode());
    map.insert(QStringLiteral("phoneTrackType"),     phoneTrackType());
    map.insert(QStringLiteral("phoneTrackUrlTemplate"),phoneTrackUrlTemplate());
    map.insert(QStringLiteral("phoneTrackSessionID"),phoneTrackSessionID());
    map.insert(QStringLiteral("phoneTrackDeviceID"), phoneTrackDeviceID());
    return map;
}

void Settings::setValue(const QString &key, const QVariant &newValue)
{
    QString storageKey;
    QVariant value = newValue;

    if (key == QStringLiteral("phoneTrackEnabled")) {
        storageKey = QString::fromLatin1(PhoneTrackEnableKey);
        value = newValue.toBool();
    } else if (key == QStringLiteral("phoneTrackLiveMode")) {
        storageKey = QString::fromLatin1(PhoneTrackLiveKey);
        value = newValue.toBool();
    } else if (key == QStringLiteral("phoneTrackUrlTemplate")) {
        storageKey = QString::fromLatin1(PhoneTrackUrlKey);
        value = newValue.toString().trimmed();
    } else if (key == QStringLiteral("phoneTrackSession")) {
        storageKey = QString::fromLatin1(PhoneTrackSessionKey);
        value = newValue.toString().trimmed();
    } else if (key == QStringLiteral("phoneTrackName")) {
        storageKey = QString::fromLatin1(PhoneTrackNameKey);
        value = newValue.toString().trimmed();
    } else if (key == QStringLiteral("phoneTrackType")) {
        storageKey = QString::fromLatin1(PhoneTrackTypeKey);
        value = newValue.value<uint>();
    } else {
        return;
    }

    if (m_settings.value(storageKey) == value) {
        return;
    }

    m_settings.setValue(storageKey, value);
    m_settings.sync();
    emit changed();
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    return m_settings.value(key, defaultValue);
}

void Settings::ensureDefaults()
{
    if (!m_settings.contains(QString::fromLatin1(PhoneTrackEnableKey))) {
        m_settings.setValue(QString::fromLatin1(PhoneTrackEnableKey), false);
    }
    m_settings.sync();
}

bool Settings::phoneTrackEnabled() const
{
    return value(QString::fromLatin1(PhoneTrackEnableKey), false).toBool();
}
bool Settings::phoneTrackLiveMode() const
{
    return value(QString::fromLatin1(PhoneTrackLiveKey), false).toBool();
}
uint Settings::phoneTrackType() const
{
    return value(QString::fromLatin1(PhoneTrackTypeKey), 0).value<uint>();
}
QString Settings::phoneTrackUrlTemplate() const
{
    return value(QString::fromLatin1(PhoneTrackUrlKey), QString()).toString();
}
QString Settings::phoneTrackSessionID() const
{
    return value(QString::fromLatin1(PhoneTrackSessionKey), "unknown").toString();
}
QString Settings::phoneTrackDeviceID() const
{
    return value(QString::fromLatin1(PhoneTrackNameKey), "").toString();
}

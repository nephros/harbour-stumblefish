// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_SETTINGS_H
#define STUMBLEFISH_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariantMap>

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);

    QString mode() const;
    bool wifiEnabled() const;
    bool cellEnabled() const;
    bool bleEnabled() const;
    bool autoUploadEnabled() const;
    bool uploadOnNonWifi() const;
    bool allowBackgroundDaemon() const;
    bool pauseActiveBackgroundOnLowBattery() const;
    bool statusNotificationsEnabled() const;
    QString endpoint() const;
    QString mapTileUrlTemplate() const;
    int reportRetentionDays() const;
    qint64 lastPruneMs() const;
    void setLastPruneMs(qint64 timestampMs);
    qint64 lastAutoUploadMs() const;
    void setLastAutoUploadMs(qint64 timestampMs);

#ifdef TRACK_MY_PHONE
    enum PhoneTrackType {
        NextCloudPhoneTrack,
        SailfishFindMyDevice, // https://sailfishos-chum.github.io/apps/harbour-find-my-device/
        Traccar,
        Custom
    };
    Q_ENUM(PhoneTrackType);

    bool phoneTrackEnabled();
    bool phoneTrackLive();
    PhoneTrackType phoneTrackType();
    QString phoneTrackUrlTemplate();
    QString phoneTrackSessionID();
    QString phoneTrackDeviceID();
#endif

    QVariantMap toMap() const;

public Q_SLOTS:
    void setValue(const QString &key, const QVariant &value);

Q_SIGNALS:
    void changed();

private:
    QVariant value(const QString &key, const QVariant &defaultValue) const;
    void ensureDefaults();

    QSettings m_settings;
};

#endif

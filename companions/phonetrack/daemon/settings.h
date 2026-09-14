// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_SETTINGS_H
#define TRACKFISH_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariantMap>
#include <QDBusInterface>

#include "config/phonetrackconfig.h"

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);

    bool phoneTrackEnabled() const;
    bool phoneTrackLiveMode() const;
    uint phoneTrackType() const;
    QString phoneTrackUrlTemplate() const;
    QString phoneTrackSessionID() const;
    QString phoneTrackDeviceID() const;


public Q_SLOTS:
    QVariantMap toMap() const;
    void applyLiveTrackConfig();
    bool canApplyLiveTrackConfig();

    void setValue(const QString &key, const QVariant &value);
    void onSettingsChanged(const QVariantMap&);

Q_SIGNALS:
    void changed();

private:
//    QVariant value(const QString &key, const QVariant &defaultValue) const;
    void updateSettings();

    QVariantMap m_settings;
    QDBusInterface *m_stumbleService;
    Stumblefish::PhoneTrackConfig* m_phoneTrackConfig;
};

#endif

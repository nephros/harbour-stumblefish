// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_SETTINGS_H
#define TRACKFISH_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariantMap>

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

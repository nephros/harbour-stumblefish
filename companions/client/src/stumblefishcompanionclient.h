// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISHCOMPANIONCLIENT_H
#define STUMBLEFISHCOMPANIONCLIENT_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>

#include "common/constants.h"

class QDBusInterface;
class QDBusPendingCallWatcher;

class StumblefishCompanionClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariantMap settings READ settings NOTIFY settingsChanged)

public:
    explicit StumblefishCompanionClient(QObject *parent = 0);
//    ~StumblefishCompanionClient();

    QVariantMap status() const;
    QVariantMap settings() const;

    Q_INVOKABLE QStringList availableCompanions() { return m_companions; };

    void registerCompanion(const QString& name);

Q_SIGNALS:
    void statusChanged();
    void settingsChanged();

private Q_SLOTS:
    void handleStatusSignal(const QVariantMap &status);
    void handleSettingsSignal(const QVariantMap &settings);

private:
    QDBusInterface *m_interface;
    QVariantMap m_status;
    QVariantMap m_settings;
    QStringList m_companions;
};

#endif

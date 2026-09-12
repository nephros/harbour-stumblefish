// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_SERVICE_H
#define TRACKFISH_SERVICE_H

#include <QObject>
#include <QDBusContext>
#include <QDBusServiceWatcher>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusVariant>
//#include <QSet>
//#include <QTimer>
#include <QVariantList>
#include <QVariantMap>

#include "settings.h"
#include "phonetrackconfig.h"
#include "trackuploader.h"


class Watcher : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.PhoneTrack")

public:
    explicit Watcher(QObject *parent = 0);
    ~Watcher();

public Q_SLOTS:
    Q_NOREPLY void applyLiveTrackConfig();
    bool canApplyLiveTrackConfig();

Q_SIGNALS:
private Q_SLOTS:
    void onReportsChanged();
    void onSettingsChanged(QVariant);
    void onStatusChanged(QVariant);
private:
    void asyncCall(const QString &method, const QVariantList &arguments, const QString &kind);
    void setSetting(const QString &key, const QVariant &value);
    QVariant getSetting(const QString &key);

    Settings m_settings;
    Stumblefish::PhoneTrackConfig* m_phoneTrackConfig;
    QDBusServiceWatcher m_daemonWatcher;
    QDBusInterface *m_interface;
    TrackUploader* m_uploader;

};

#endif

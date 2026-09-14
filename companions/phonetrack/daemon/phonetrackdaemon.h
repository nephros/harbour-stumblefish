// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_SERVICE_H
#define TRACKFISH_SERVICE_H

#include "base/companionbase.h"
#include <QObject>
#include <QSettings>
#include <QDebug>

#include "config/phonetrackconfig.h"
#include "trackuploader.h"

class Companion : public StumblefishCompanionBase
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Tracker")

public:
    explicit Companion(QObject *parent = 0);

public Q_SLOTS:
    bool phoneTrackEnabled();
    bool phoneTrackLiveEnabled();
    uint phoneTrackType();
    Q_NOREPLY void setPhoneTrackEnabled(bool enable);
    Q_NOREPLY void setPhoneTrackLiveEnabled(bool enable);
    Q_NOREPLY void setPhoneTrackType(uint type);

    bool canApplyLiveTrackConfig();
    Q_NOREPLY void applyLiveTrackConfig();

Q_SIGNALS:
private Q_SLOTS:
    void handleDBusMethod() override; // Implement pure virtual method
    void onStumblefishVanished(const QString&);

    QVariantMap status() const;
    QVariantMap settings() const;

    void onReportsChanged();
    void onSettingsChanged(const QVariantMap&);
    void onStatusChanged(const QVariantMap&);

private:
    //void asyncCall(const QString &method, const QVariantList &arguments, const QString &kind);
    //void setSetting(const QString &key, const QVariant &value);
    //QVariant getSetting(const QString &key);
    void getReport(int reportId = 0);
    unsigned long int m_lastReport = 0;

    QDBusInterface *m_stumbleService;
    QDBusServiceWatcher m_stumbleWatcher;

    QSettings m_settings;
    Stumblefish::PhoneTrackConfig* m_phoneTrackConfig;
    TrackUploader m_uploader;
};

#endif

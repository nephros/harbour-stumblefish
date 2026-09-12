// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_SERVICE_H
#define TRACKFISH_SERVICE_H

#include <QObject>
#include <QDBusContext>
#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QVariantList>
#include <QVariantMap>

#include "settings.h"
#include "companions/common/constants.h"
#include "trackuploader.h"


class Companion : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Tracker")

public:
    explicit Companion(QObject *parent = 0);
    ~Companion();

public Q_SLOTS:

    Q_NOREPLY void applyLiveTrackConfig();
    bool canApplyLiveTrackConfig();

Q_SIGNALS:
private Q_SLOTS:
    QVariantMap status() const;
    QVariantMap settings() const;

    void onReportsChanged();
    void onSettingsChanged(QVariant);
    void onStatusChanged(QVariant);
private:
    //void asyncCall(const QString &method, const QVariantList &arguments, const QString &kind);
    //void setSetting(const QString &key, const QVariant &value);
    //QVariant getSetting(const QString &key);
    void getReport(int reportId = 0);

    QDBusInterface *m_stumbleService;
    QDBusServiceWatcher m_stumbleWatcher;

    Settings m_settings;
    TrackUploader m_uploader;
    Stumblefish::PhoneTrackConfig* m_phoneTrackConfig;


};

#endif

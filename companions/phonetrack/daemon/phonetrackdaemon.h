// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_SERVICE_H
#define TRACKFISH_SERVICE_H


#include "companions/common/abstractcompanion.h"

#include <QObject>

#include "companions/common/constants.h"
#include "config/phonetrackconfig.h"
#include "settings.h"
#include "trackuploader.h"

class Companion : public StumblefishCompanion
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Tracker")

public:
    explicit Companion(QObject *parent = 0);
//    explicit Companion(StumblefishCompanion *other);
//    ~Companion();

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

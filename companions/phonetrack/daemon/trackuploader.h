// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_UPLOADER_H
#define TRACKFISH_UPLOADER_H

#include <QObject>
#include <QList>
#include <QDBusPendingCallWatcher>

//#include "daemon/observations.h"
#include "trackreport.h"

class QNetworkAccessManager;
class QNetworkReply;

class TrackUploader : public QObject
{
    Q_OBJECT

public:
    explicit TrackUploader(QObject *parent = 0);

    bool uploading() const;

public Q_SLOTS:
    void uploadTracked(const Trackfish::Report& report, const QVariantMap& settings);
    void reportsHandler(QDBusPendingCallWatcher* watcher);

Q_SIGNALS:
    void uploadFinished(bool success, const QString &message);

private Q_SLOTS:

private:
    QUrl buildTrackingUrl(Trackfish::Report &report, int includedId) const;
    QUrl formatTrackingUrl(unsigned int trackType, const QUrl& tpl,
                            const QString& session,
                            const QString& device,
                            const Trackfish::Report& report,
                            const QString& ua) const;

    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;
    QVariantMap m_uploadSettings;
    QList<Trackfish::Report> m_uploadCandidates;
    QList<int> m_uploadingIds;
};

#endif

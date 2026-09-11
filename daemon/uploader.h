// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_UPLOADER_H
#define STUMBLEFISH_UPLOADER_H

#include <QObject>
#include <QList>

#include "observations.h"

class QNetworkAccessManager;
class QNetworkReply;
class Settings;
class Storage;

class Uploader : public QObject
{
    Q_OBJECT

public:
    explicit Uploader(Storage *storage, Settings *settings, QObject *parent = 0);

    bool uploading() const;

public Q_SLOTS:
    void uploadPending();
    void uploadAutomatically();
    void retryReport(int reportId);
#ifdef TRACK_MY_PHONE
    void uploadTracked(const Report& report);
#endif

Q_SIGNALS:
    void uploadFinished(bool success, const QString &message);
#ifdef TRACK_MY_PHONE
    void trackingFinished(bool success, const QString &message);
#endif

private Q_SLOTS:
    void replyFinished();

private:
    void uploadPending(int maxRetryCount);
    QByteArray buildPayload(const QList<Report> &reports, QList<int> *includedIds) const;
#ifdef TRACK_MY_PHONE
    QUrl buildTrackingUrl(Report &report, int includedId) const;
    QUrl formatTrackingUrl(unsigned int trackType, const QUrl& tpl,
                            const QString& session,
                            const QString& device,
                            const Report& report,
                            const QString& ua) const;
#endif

    Storage *m_storage;
    Settings *m_settings;
    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;
    QList<int> m_uploadingIds;
};

#endif

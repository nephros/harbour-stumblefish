// SPDX-License-Identifier: MIT
#include "trackuploader.h"

#include "companions/base/constants.h"
#include "config/phonetrackconfig.h"
//#include "daemon/observations.h"

//#include <QJsonArray>
//#include <QJsonDocument>
//#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDBusMessage>

#include <climits>

namespace {
const char NCPhoneTrackAppUri[] = "/apps/phonetrack";
const int AutomaticMaxRetries = 5;
}

TrackUploader::TrackUploader(QObject *parent)
    : QObject(parent)
    , m_network(new QNetworkAccessManager(this))
    , m_reply(0)
{
}

void TrackUploader::reportsHandler(QDBusPendingCallWatcher* watcher)
{
    QDBusMessage reply = watcher->reply();
    if (reply.type() == QDBusMessage::ReplyMessage) {
    } else {
        qDebug() << "Error:" << reply.errorName() << reply.errorMessage();
    }
    watcher->deleteLater();
}

QUrl TrackUploader::formatTrackingUrl(uint trackType, const QUrl& tpl,
                            const QString& session,
                            const QString& device,
                            const Trackfish::Report& report,
                            const QString& ua) const
{
    QUrl url(tpl);
    QUrlQuery q(url.query());
    QString path = url.path();

    Stumblefish::PhoneTrackConfig conf;
    if(trackType == conf.info(conf.defaultConfig())->id) { // nextcloud phoneTrack
        path.append(QString::fromLatin1(NCPhoneTrackAppUri));
        path.append("/" + session);
        if (!device.isEmpty())
            path.append("/" + device);
    } else if(trackType == conf.info("traccar")->id) {
        q.addQueryItem(QStringLiteral("id"), session);
    } else if(trackType == conf.info("custom")->id) {
        QString qs = q.toString();
        qs.replace(QStringLiteral("{latitude}"), QString::number(report.position.latitude));
        qs.replace(QStringLiteral("{longitude}"), QString::number(report.position.longitude));
        qs.replace(QStringLiteral("{useragent}"), QUrl::toPercentEncoding(ua));
        qs.replace(QStringLiteral("{session}"), session);
        url.setPath(path);
        url.setQuery(QUrlQuery(qs));
        return url;
    }
    q.addQueryItem(QStringLiteral("lat"), QString::number(report.position.latitude));
    q.addQueryItem(QStringLiteral("lon"), QString::number(report.position.longitude));
    q.addQueryItem(QStringLiteral("alt"), QString::number(report.position.altitude));
    q.addQueryItem(QStringLiteral("acc"), QString::number(report.position.accuracy));
    if (report.position.speed != DBL_MAX)
        q.addQueryItem(QStringLiteral("speed"), QString::number(report.position.speed));
    if (report.position.direction != DBL_MAX)
        q.addQueryItem(QStringLiteral("bearing"), QString::number(report.position.direction));
    q.addQueryItem(QStringLiteral("sat"), QString::number(report.position.satellites));

    q.addQueryItem(QStringLiteral("bat"), QString::number(report.battery));
    q.addQueryItem(QStringLiteral("timestamp"), QString::number(static_cast<double>(report.timestampMs/1000)));
    q.addQueryItem(QStringLiteral("useragent"), QUrl::toPercentEncoding(ua));

    url.setPath(path);
    url.setQuery(q);
    return url;
}

void TrackUploader::uploadTracked(const Trackfish::Report& report, const QVariantMap& settings)
{
    qDebug() << Q_FUNC_INFO;
    QUrl url = formatTrackingUrl(settings.value("phoneTrackType").value<uint>(), settings.value("phoneTrackUrlTemplate").toString(),
                                 settings.value("phoneTrackSessionID").toString(),
                                 settings.value("phoneTrackDeviceID").toString(),
                                 report,
                                 Trackfish::UserAgent.toUtf8()
                            );
     if (!url.isValid() || url.scheme().isEmpty() || url.host().isEmpty()) {
         emit uploadFinished(false, QStringLiteral("Phone track endpoint is invalid"));
         qDebug() << QStringLiteral("Phone track endpoint is invalid") << settings.value("phoneTrackUrlTemplate").toString();
         return;
    }
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", Trackfish::UserAgent.toUtf8());
    qDebug() << Q_FUNC_INFO << "uploading";
    m_network->get(request);
}

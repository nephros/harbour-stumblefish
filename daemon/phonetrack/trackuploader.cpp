// SPDX-License-Identifier: MIT
#include "trackuploader.h"

#include "common/constants.h"
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

bool TrackUploader::uploading() const
{
    return m_reply != 0;
}

void TrackUploader::uploadPending()
{
    uploadPending(-1);
}

void TrackUploader::uploadAutomatically()
{
    uploadPending(AutomaticMaxRetries);
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
void TrackUploader::uploadPending(int maxRetryCount)
{
    if (m_reply) {
        emit uploadFinished(false, QStringLiteral("Upload already in progress"));
        return;
    }

    const QList<Report> reports = m_uploadCandidates;
    if (reports.isEmpty()) {
        emit uploadFinished(true, QStringLiteral("No pending reports"));
        return;
    }

    // TODO: do we want to check for live here? even in live mode we may want
    // to upload pending ones...
    // FIXME: dedup already uploaded ones (thu live mode)
//    if (m_settings->phoneTrackEnabled() && !m_settings->phoneTrackLiveMode()) {
        foreach (const Report &report, reports) {
            uploadTracked(report, m_uploadSettings);
        }
//    }
   connect(m_reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void TrackUploader::retryReport(int reportId)
{
//    m_storage->markPending(reportId);
    uploadPending();
}

void TrackUploader::replyFinished()
{
    QNetworkReply *reply = m_reply;
    m_reply = 0;

    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QByteArray body = reply->readAll();
    QString message;
    bool success = false;

    if (reply->error() == QNetworkReply::NoError && status >= 200 && status < 300) {
        success = true;
        message = QStringLiteral("Uploaded %1 reports").arg(m_uploadingIds.count());
        //m_storage->markUploaded(m_uploadingIds);
    } else {
        message = reply->errorString();
        if (!body.isEmpty()) {
            message += QStringLiteral(": ") + QString::fromUtf8(body.left(300));
        }
        if (message.trimmed().isEmpty()) {
            message = QStringLiteral("HTTP %1").arg(status);
        }
        //m_storage->markFailed(m_uploadingIds, message);
    }
    reply->deleteLater();
    m_uploadingIds.clear();
    emit uploadFinished(success, message);
}

QUrl TrackUploader::formatTrackingUrl(uint trackType, const QUrl& tpl,
                            const QString& session,
                            const QString& device,
                            const Report& report,
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


/*
QByteArray TrackUploader::buildPayload(const QList<Report> &reports, QList<int> *includedIds) const
{
    QJsonArray items;
    foreach (const Report &report, reports) {
        QJsonObject item;
        item.insert(QStringLiteral("timestamp"), static_cast<double>(report.timestampMs));

        QJsonObject position;
        position.insert(QStringLiteral("latitude"), report.position.latitude);
        position.insert(QStringLiteral("longitude"), report.position.longitude);
        if (report.position.altitude == report.position.altitude) {
            position.insert(QStringLiteral("altitude"), report.position.altitude);
        }
        position.insert(QStringLiteral("accuracy"), report.position.accuracy);
        item.insert(QStringLiteral("position"), position);

        QJsonArray wifiAccessPoints;
        foreach (const WifiObservation &wifi, report.wifi) {
            if (wifi.macAddress.isEmpty() || wifi.ssid.isEmpty()) {
                continue;
            }

            QJsonObject object;
            object.insert(QStringLiteral("macAddress"), wifi.macAddress);
            object.insert(QStringLiteral("ssid"), wifi.ssid);
            if (wifi.frequency > 0) {
                object.insert(QStringLiteral("frequency"), wifi.frequency);
            }
            if (wifi.signalStrength < 0) {
                object.insert(QStringLiteral("signalStrength"), wifi.signalStrength);
            }
            const int observationAge = age(report.timestampMs, wifi.seenMs);
            if (observationAge > MaxWifiObservationAgeMs) {
                continue;
            }
            if (observationAge > 0) {
                object.insert(QStringLiteral("age"), observationAge);
            }
            wifiAccessPoints.append(object);
        }
        if (!wifiAccessPoints.isEmpty()) {
            item.insert(QStringLiteral("wifiAccessPoints"), wifiAccessPoints);
        }

        QJsonArray cellTowers;
        foreach (const CellObservation &cell, report.cells) {
            if (!hasEnoughCellData(cell)) {
                continue;
            }

            QJsonObject object;
            object.insert(QStringLiteral("radioType"), cell.radioType);
            insertPositiveUint16(&object, QStringLiteral("mobileCountryCode"),
                                 cell.mobileCountryCode);
            insertUint16(&object, QStringLiteral("mobileNetworkCode"),
                         cell.mobileNetworkCode);
            insertLocationAreaCode(&object, QStringLiteral("locationAreaCode"),
                                   cell.locationAreaCode);
            if (isPositiveCellId(cell.cellId)) {
                object.insert(QStringLiteral("cellId"), static_cast<double>(cell.cellId));
            }
            insertUint16(&object, QStringLiteral("primaryScramblingCode"),
                         cell.primaryScramblingCode);
            insertKnownValue(&object, QStringLiteral("asu"), cell.asu);
            insertKnownValue(&object, QStringLiteral("timingAdvance"), cell.timingAdvance);
            insertKnownValue(&object, QStringLiteral("arfcn"), cell.arfcn);
            if (cell.signalStrength < 0) {
                object.insert(QStringLiteral("signalStrength"), cell.signalStrength);
            }
            object.insert(QStringLiteral("serving"), cell.serving ? 1 : 0);
            cellTowers.append(object);
        }
        if (!cellTowers.isEmpty()) {
            item.insert(QStringLiteral("cellTowers"), cellTowers);
        }

        QJsonArray bluetoothBeacons;
        foreach (const BleObservation &ble, report.ble) {
            QJsonObject object;
            object.insert(QStringLiteral("macAddress"), ble.macAddress);
            if (!ble.name.isEmpty()) {
                object.insert(QStringLiteral("name"), ble.name);
            }
            if (ble.signalStrength < 0) {
                object.insert(QStringLiteral("signalStrength"), ble.signalStrength);
            }
            const bool hasBeaconIdentifiers = !ble.id1.isEmpty() || !ble.id2.isEmpty() || !ble.id3.isEmpty();
            if (ble.beaconType >= 0 && (ble.beaconType != 0 || hasBeaconIdentifiers)) {
                object.insert(QStringLiteral("beaconType"), ble.beaconType);
            }
            if (!ble.id1.isEmpty()) {
                object.insert(QStringLiteral("id1"), ble.id1);
            }
            if (!ble.id2.isEmpty()) {
                object.insert(QStringLiteral("id2"), ble.id2);
            }
            if (!ble.id3.isEmpty()) {
                object.insert(QStringLiteral("id3"), ble.id3);
            }
            const int observationAge = age(report.timestampMs, ble.seenMs);
            if (observationAge > MaxBleObservationAgeMs) {
                continue;
            }
            if (observationAge > 0) {
                object.insert(QStringLiteral("age"), observationAge);
            }
            bluetoothBeacons.append(object);
        }
        if (!bluetoothBeacons.isEmpty()) {
            item.insert(QStringLiteral("bluetoothBeacons"), bluetoothBeacons);
        }

        if (wifiAccessPoints.count() < 2 && cellTowers.isEmpty() && bluetoothBeacons.isEmpty()) {
            continue;
        }

        if (includedIds) {
            includedIds->append(report.id);
        }
        items.append(item);
    }

    QJsonObject root;
    root.insert(QStringLiteral("items"), items);
    return QJsonDocument(root).toJson(QJsonDocument::Compact);
}
*/
void TrackUploader::uploadTracked(const Report& report, const QVariantMap& settings)
{
    QUrl url = formatTrackingUrl(settings.value("phoneTrackType").value<uint>(), settings.value("phoneTrackUrlTemplate").toString(),
                                 settings.value("phoneTrackSessionID").toString(),
                                 settings.value("phoneTrackDeviceID").toString(),
                                 report,
                                 Trackfish::UserAgent
                            );
     if (!url.isValid() || url.scheme().isEmpty() || url.host().isEmpty()) {
         emit uploadFinished(false, QStringLiteral("Phone track endpoint is invalid"));
         return;
    }
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", Trackfish::UserAgent);
    m_network->get(request);

}

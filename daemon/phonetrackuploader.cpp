// SPDX-License-Identifier: MIT
#include "phonetrackuploader.h"
#include <QUrl>
#include <QUrlQuery>

QUrl PhonetrackUploader::formatSubmitUrl(const enum ServiceId service, const QString& id, const QVariantMap& positionData) const
{
    QUrl url;
    QUrlQuery query;
    url.setScheme("https");
    url.setHost(m_settings->trackEndpoint());
    if(service == ServiceId::Traccar) {
        query.addQueryItem("id", id);
    } else {
        url.setPath("/id=" + id);
    }
    query.addQueryItem("timestamp", QString(positionData.value("timestamp").toInt()));
    query.addQueryItem("lat",  QString(positionData.value("latitude").toInt()));
    query.addQueryItem("lon", QString(positionData.value("longitud").toInt()));
    query.addQueryItem("alt", QString(positionData.value("altitude").toInt()));
    query.addQueryItem("speed", QString(positionData.value("speed").toInt()));
    query.addQueryItem("acc", QString(positionData.value("accuracy").toInt()));
    if(service == ServiceId::Nextcloud) {
        query.addQueryItem("useragent", m_settings->userAgent());
    }

    url.setQuery(query);
    return url;
}

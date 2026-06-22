#include "glassfish.h"
#include "ids.h"

#include "../common/constants.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusError>
#include <QDateTime>
#include <QDebug>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

static const QList<int> manufacturerIds = {
    1177,
    1371,
    13875,
    2291,
    2362,
    2400,
    756
};

static QJsonDocument readBeaconData()
{
    Q_INIT_RESOURCE(fingerprints);
    QString val;
    QFile file(QStringLiteral(":data/fingerprints.json"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    qDebug() << qPrintable(val);
    const QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    return d;
}

bool Glassfish::collectingEnabled()
{
    return checkBleEnabled();
}

bool Glassfish::checkBleEnabled() const
{
    QVariantMap result;
    QDBusMessage message = QDBusMessage::createMethodCall(
                                Stumblefish::ServiceName,
                                Stumblefish::ObjectPath,
                                Stumblefish::InterfaceName,
                                QStringLiteral("settings")
    );
    QDBusReply<QVariantMap> reply = QDBusConnection::sessionBus().call(message);
    if (reply.isValid()) {
        result = reply.value();
    } else {
        qDebug() << Q_FUNC_INFO << "DBus Error:" << reply.error().message();
    }
    return result.value("bleEnabled").toBool();
}


QVariantMap Glassfish::callReport() const
{
    QVariantMap result;
    QDBusMessage message = QDBusMessage::createMethodCall(
                                Stumblefish::ServiceName,
                                Stumblefish::ObjectPath,
                                Stumblefish::InterfaceName,
                                QStringLiteral("report")
    );
    QList<QVariant> args;
    args << QVariant(0);
    message.setArguments(args);
    QDBusReply<QVariantMap> reply = QDBusConnection::sessionBus().call(message);
    /*
     * busctl --user call org.stumblefish.Collector /org/stumblefish/Collector  org.stumblefish.Collector report i 0
     * a{sv} 21 "accuracy" d -1 "altitude" d 0 "ble" av 0 "bleCount" i 0 "bleEnabled" b false "cellCount" i 0 "cellEnabled" b fals
     * e "cells" av 0 "endpoint" s "" "id" i 0 "lastError" s "" "latitude" d 0 "longitude" d 0 "mode" s "" "retryCount" i 0 "times
     * tampMs" x 0 "uploadStatus" s "" "uploadedAtMs" x 0 "wifi" av 0 "wifiCount" i 0 "wifiEnabled" b false
     */
    if (reply.isValid()) {
        result = reply.value();
    } else {
        qDebug() << Q_FUNC_INFO << "DBus Error:" << reply.error().message();
    }
    return result;
}

QList<QVariantMap> Glassfish::getReports(int limit) const
{
    QList<QVariantMap> result;

    QDBusMessage message = QDBusMessage::createMethodCall(
                                Stumblefish::ServiceName,
                                Stumblefish::ObjectPath,
                                Stumblefish::InterfaceName,
                                QStringLiteral("reports")
    );
    QList<QVariant> args;
    args << QVariant(limit); // limit
    message.setArguments(args);
    QDBusReply<QVariantList> reply = QDBusConnection::sessionBus().call(message);
    //if ((reply.isValid()) && (reply.value().count() > 0)) {
    if (reply.isValid()) {
        //return reply.value();
        for ( const QVariant &entry : reply.value() ) {
            QVariantMap map = entry.toMap();
            if (map.value("bleCount", 0).toInt() != 0) {
                result.append(map);
            }
        }
    } else {
        qDebug() << Q_FUNC_INFO << "DBus Error:" << reply.error().message();
    }
    return result;
}

void Glassfish::analyzeReports()
{
    const int maxreports = 12;
    const qint64 cutoffms = 60*1000;
    // rssi -60 ≈ 3 m.
    const int min_rssi = 60;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();

    const QList<QVariantMap> list = getReports(maxreports);
    for (const auto &report : list) {
        if (report.value("bleReports").toInt() == 0) continue;
        const QVariantList beacons = report.value("ble").toList();
        for ( const QVariant& entry : beacons) {
            const QVariantMap beacon = entry.toMap();
            if ((now - beacon.value("seenMs").toInt()) > cutoffms) continue;
            if (beacon.value("signalStrength").toInt() < min_rssi) continue;
            const int mfgId = beacon.value("manufacturerData").toString().toInt();
            if(manufacturerIds.contains(mfgId)) {
                emit alert();
            }
        }
    }
}

QJsonObject Glassfish::manufacturerForId(int id)
{
    if (beaconData.isEmpty()) {
        beaconData = readBeaconData();
        if (beaconData.isEmpty()) { qCritical() << "could not load beacon info!"; }
    }
    QJsonArray prints = beaconData.object().value("fingerprints").toArray();
    for (int i = 0; i < prints.size(); ++i) {
        QJsonValue value = prints.at(i);
        if (value.toArray().contains(id)) return value.toObject();
    }
    return QJsonObject();
}


// vim: expandtab ts=4 sw=4 st=4

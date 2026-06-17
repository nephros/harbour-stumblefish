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

static QJsonDocument readBeaconData()
{
    Q_INIT_RESOURCE(fingerprints);
    QString val;
    QFile file(QStringLiteral(":data/fingerprints.json"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
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
    if (beaconData.isEmpty()) {
        beaconData = readBeaconData();
    }

    const qint64 cutoffms = 60*1000;
    const int min_rssi = 50;
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList<QVariantMap> list = getReports(12);
    for (const auto &report : list) {
        if (report.value("bleReports").toInt() == 0) continue;
        QVariantList beacons = report.value("ble").toList();
        for ( const QVariant& entry : beacons) {
            QVariantMap beacon = entry.toMap();
            if ((now - beacon.value("seenMs").toInt()) > cutoffms) continue;
            if (beacon.value("signalStrength").toInt() == 0) continue;
            if (beacon.value("signalStrength").toInt() > min_rssi) continue;
            QString mfgData = beacon.value("manufacturerData").toString();
        }
    }
}
// vim: expandtab ts=4 sw=4 st=4

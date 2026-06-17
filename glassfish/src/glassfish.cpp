#include "glassfish.h"
#include "ids.h"

#include "../common/constants.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusError>
#include <QDebug>

bool Glassfish::bleCollectionEnabled()
{
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
        qDebug() << "reply was valid" << reply.value().value("bleEnabled");
        return reply.value().value("bleEnabled").toBool();
    } else {
        qDebug() <<  QDBusConnection::sessionBus().lastError().message();
        qDebug() <<  reply.error();
    }
    return false;
}

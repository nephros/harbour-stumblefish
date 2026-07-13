#include "jollapass.h"

#include "qble/qblelocalapplication.h"
#include "qble/qblelocalservice.h"
#include "qble/qblelocalcharacteristic.h"

#include <QDebug>

using namespace Stumblefish;
using namespace JollaPass;

void PassService::start(const QString& adapter) {
    auto bus = // haha
               QDBusConnection::systemBus();
    QBLELocalApplication bleapp(BTLE_JOLLAPASS_APP_PATH);
    QBLELocalService svc(bus, 0, BTLE_JOLLAPASS_SERVICE_ID, BTLE_JOLLAPASS_SERVICE_PATH);
    QStringList flags = { "read", "broadcast" };
    QBLELocalCharacteristic c1(bus, 0, BTLE_JOLLAPASS_CHARACTERISTIC1_ID, flags, &svc);
    QBLELocalCharacteristic c2(bus, 0, BTLE_JOLLAPASS_CHARACTERISTIC2_ID, flags, &svc);
    bleapp.addService(&svc);
    bleapp.registerWithAdapter(adapter);
}

QByteArray PassCharacteristic::ReadValue(const QVariantMap&)
{
    return QByteArrayLiteral("enabled");
}

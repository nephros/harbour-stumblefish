#include "jollapass.h"

#include "qble/qblelocalapplication.h"
#include "qble/qblelocalservice.h"
#include "qble/qblelocalcharacteristic.h"

#include <QDebug>

using namespace Stumblefish;
using namespace JollaPass;

void PassService::update(const QString& adapter) {
    QBLELocalApplication bleapp(BTLE_JOLLAPASS_APP_PATH);
    if (!m_adapterPath.isNull() && (adapter != m_adapterPath)) {
       bleapp.unregisterFromAdapter(m_adapterPath);
    }
    auto bus = // haha
               QDBusConnection::systemBus();
    QBLELocalService svc(bus, 0, BTLE_JOLLAPASS_SERVICE_ID, BTLE_JOLLAPASS_SERVICE_PATH);
    QStringList flags = { "read", "broadcast" };
    QBLELocalCharacteristic c1(bus, 0, BTLE_JOLLAPASS_CHARACTERISTIC1_ID, flags, &svc);
    QBLELocalCharacteristic c2(bus, 0, BTLE_JOLLAPASS_CHARACTERISTIC2_ID, flags, &svc);
    bleapp.addService(&svc);
    bleapp.registerWithAdapter(adapter);
    m_adapterPath = adapter;
}

QByteArray PassCharacteristic::ReadValue(const QVariantMap&)
{
    return QByteArrayLiteral("enabled");
}

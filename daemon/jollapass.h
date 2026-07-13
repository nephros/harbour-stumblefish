#ifndef JOLLAPASS_H
#define JOLLAPASS_H
#include <QString>
#include "qble/qblelocalcharacteristic.h"

#ifdef FIND_JOLLA_BUDDIES
const QString BT_VENDOR_JOLLA = "50:56:A8"; // since 2013 ;)
const QString BTLE_JOLLAPASS_SERVICE_ID        = "056AB00-AAF7-406B-806F-1732F37AB8CA";
const QString BTLE_JOLLAPASS_CHARACTERISTIC1_ID = "056AB01-AAF7-406B-806F-1732F37AB8CA";
const QString BTLE_JOLLAPASS_CHARACTERISTIC2_ID = "056AB02-AAF7-406B-806F-1732F37AB8CA";
const QString BTLE_JOLLAPASS_APP_PATH = "/org/sailfishos/JollaPass1";
const QString BTLE_JOLLAPASS_SERVICE_PATH = "/org/sailfishos/JollaPass1/service";

class PassService
{
public:
    void start(const QString& adapter = "/org/bluez/hci0");
};

class PassCharacteristic : public QBLELocalCharacteristic
{
public:
    QByteArray ReadValue(const QVariantMap &options) override;
};
#endif
#endif // JOLLAPASS_H

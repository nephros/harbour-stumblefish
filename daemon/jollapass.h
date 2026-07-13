#ifndef JOLLAPASS_H
#define JOLLAPASS_H
#include "../common/constants.h"
#include "qble/qblelocalcharacteristic.h"

#ifdef FIND_JOLLA_BUDDIES
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

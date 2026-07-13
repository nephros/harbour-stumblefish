#ifndef JOLLAPASS_H
#define JOLLAPASS_H
#include "../common/constants.h"
#include "qble/qblelocalcharacteristic.h"

#ifdef FIND_JOLLA_BUDDIES
class PassService
{
public:
    void update(const QString& adapter = "/org/bluez/hci0");
private:
    QString m_adapterPath;
};


class PassCharacteristic : public QBLELocalCharacteristic
{
public:
    QByteArray ReadValue(const QVariantMap &options) override;
};

#endif
#endif // JOLLAPASS_H

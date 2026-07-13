#ifndef JOLLAPASS_H
#define JOLLAPASS_H
#include <QString>

#ifdef FIND_JOLLA_BUDDIES
const QString BT_VENDOR_JOLLA = "50:56:A8"; // since 2013 ;)
const QString BTLE_JOLLAPASS_SERVICE_ID        = "ebd18207-aaf7-406b-806f-1732f37ab8ca";
const QString BTLE_JOLLAPASS_CHARACTERISTIC_ID = "0433924c-0927-4076-bb77-9359ac3994e0";

namespace {
class BLEService
{
public:
    void start();
};
} // ns
#endif
#endif // JOLLAPASS_H

// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_CONSTANTS_H
#define STUMBLEFISH_CONSTANTS_H

#include <QString>

namespace Stumblefish {

const char ServiceName[] = "org.stumblefish.Collector";
const char ObjectPath[] = "/org/stumblefish/Collector";
const char InterfaceName[] = "org.stumblefish.Collector";
const char DefaultEndpoint[] = "https://api.beacondb.net/v2/geosubmit";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "Stumblefish";
const char BinaryName[] = "harbour-stumblefish";

#ifdef FIND_JOLLA_BUDDIES
namespace JollaPass {

const QString BT_VENDOR_JOLLA = "50:56:A8"; // since 2013 ;)
const QString BTLE_JOLLAPASS_SERVICE_ID        = "056AB00-AAF7-406B-806F-1732F37AB8CA";
const QString BTLE_JOLLAPASS_CHARACTERISTIC1_ID = "056AB01-AAF7-406B-806F-1732F37AB8CA";
const QString BTLE_JOLLAPASS_CHARACTERISTIC2_ID = "056AB02-AAF7-406B-806F-1732F37AB8CA";
const QString BTLE_JOLLAPASS_APP_PATH = "/org/stumblefish/JollaPass1";
const QString BTLE_JOLLAPASS_SERVICE_PATH = "/org/stumblefish/JollaPass1/service";
}
#endif

}
#endif

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
const char ApplicationName[] = "harbour-stumblefish";

}

#ifdef TRACK_MY_PHONE
#include "phonetrackconfig.h"
namespace Trackfish {

const char ServiceName[] = "org.stumblefish.Tracker";
const char ObjectPath[] = "/org/stumblefish/Tracker";
const char InterfaceName[] = "org.stumblefish.Tracker";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "harbour-trackfish";
const QByteArray UserAgent = QStringLiteral("harbour-stumblefish/%1 (%2)")
                             .arg(QStringLiteral(APP_VERSION))
                             .arg(QStringLiteral("PhoneTrack Companion"))
                             .toUtf8();
}

#endif

#endif

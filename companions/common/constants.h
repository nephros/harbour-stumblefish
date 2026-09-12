// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_COMPANION_CONSTANTS_H
#define STUMBLEFISH_COMPANION_CONSTANTS_H

#include <QString>
#include "versions.h"
#include "common/constants.h"

#ifdef TRACK_MY_PHONE
namespace Trackfish {

const char ServiceName[] = "org.stumblefish.Tracker";
const char ObjectPath[] = "/org/stumblefish/Tracker";
const char InterfaceName[] = "org.stumblefish.Tracker";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "harbour-trackfish";
const QString UserAgent = QStringLiteral("harbour-stumblefish/%1 (%2)")
                             .arg(QStringLiteral(TRACKFISH_VERSION))
                             .arg(QStringLiteral("PhoneTrack Companion"));
} //namespace
#endif //TRACK_MY_PHONE

#endif

// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_COMPANION_CONSTANTS_H
#define STUMBLEFISH_COMPANION_CONSTANTS_H

#include <QString>
#include "versions.h"

#ifdef TRACK_MY_PHONE
namespace Trackfish {

const char ServiceName[] = "org.stumblefish.Companions";
const char ObjectPath[] = "/org/stumblefish/Tracker";
const char InterfaceName[] = "org.stumblefish.Tracker";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "harbour-trackfish";
const QString UserAgent = QStringLiteral("harbour-stumblefish/%1 (%2)")
                             .arg(QStringLiteral(TRACKFISH_VERSION))
                             .arg(QStringLiteral("PhoneTrack Companion"));
} //namespace
#endif //TRACK_MY_PHONE

#ifdef FIND_JOLLA_BUDDIES
namespace Jollapass {

const char ServiceName[] = "org.stumblefish.Companions";
const char ObjectPath[] = "/org/stumblefish/JollaPass";
const char InterfaceName[] = "org.stumblefish.JollaPass";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "harbour-jollapass";
const QString UserAgent = QStringLiteral("harbour-stumblefish/%1 (%2)")
                             .arg(QStringLiteral(JOLLAPASS_VERSION))
                             .arg(QStringLiteral("JollaPass Companion"));
} //namespace
#endif //FIND_JOLLA_BUDDIES

#ifdef FIND_KLABAUTERS
namespace Glassfish {

const char ServiceName[] = "org.stumblefish.Companions";
const char ObjectPath[] = "/org/stumblefish/Lookout";
const char InterfaceName[] = "org.stumblefish.Lookout";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "harbour-glassfish";
const QString UserAgent = QStringLiteral("harbour-stumblefish/%1 (%2)")
                             .arg(QStringLiteral(GLASSFISH_VERSION))
                             .arg(QStringLiteral("Glassfish Companion"));
} //namespace
#endif //FIND_KLABAUTERS


#endif

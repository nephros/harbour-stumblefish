// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_COMPANION_CONSTANTS_H
#define STUMBLEFISH_COMPANION_CONSTANTS_H

#include <QString>
#include "common/constants.h"

static QString UserAgentBase = QStringLiteral("%1/%2")
            .arg(QString::fromLatin1(Stumblefish::ApplicationName))
            .arg(QString::fromLatin1(APP_VERSION));

namespace Stumblefish {
const char CompanionAppName[] = "StumbleCompanion";
}

#ifdef TRACK_MY_PHONE
namespace Trackfish {

const char ServiceName[] = "org.stumblefish.Companions";
const char ObjectPath[] = "/org/stumblefish/Tracker";
const char InterfaceName[] = "org.stumblefish.Tracker";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "TrackFish";
const QString UserAgent = QStringLiteral("%1 (%2 Companion)")
                             .arg(UserAgentBase)
                             .arg(QString::fromLatin1(ApplicationName));
} //namespace
#endif //TRACK_MY_PHONE

#ifdef FIND_JOLLA_BUDDIES
namespace Jollapass {

const char ServiceName[] = "org.stumblefish.Companions";
const char ObjectPath[] = "/org/stumblefish/JollaPass";
const char InterfaceName[] = "org.stumblefish.JollaPass";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "JollaPass";
const QString UserAgent = QStringLiteral("%1 (%2 Companion)")
                             .arg(UserAgentBase)
                             .arg(QString::fromLatin1(ApplicationName));
} //namespace
#endif //FIND_JOLLA_BUDDIES

#ifdef FIND_KLABAUTERS
namespace Glassfish {

const char ServiceName[] = "org.stumblefish.Companions";
const char ObjectPath[] = "/org/stumblefish/Lookout";
const char InterfaceName[] = "org.stumblefish.Lookout";
const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "GlassFish";
const QString UserAgent = QStringLiteral("%1 (%2 Companion)")
                             .arg(UserAgentBase)
                             .arg(QString::fromLatin1(ApplicationName));
} //namespace
#endif //FIND_KLABAUTERS


#endif

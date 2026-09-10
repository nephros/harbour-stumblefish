// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_PHONETRACK_H
#define STUMBLEFISH_PHONETRACK_H

#include <QObject>

namespace Stumblefish {

class PhoneTrack : public QObject
{
    Q_GADGET
public:
    enum Type {
        NextCloudPhoneTrack,    // https://github.com/julien-nc/phonetrack/blob/main/doc/user.md
        SailfishFindMyDevice,   // https://sailfishos-chum.github.io/apps/harbour-find-my-device/
        Traccar,
        OsmAnd,
        GpsTracker,             // https://github.com/nickfox/GpsTracker
        Custom
    };
    Q_ENUM(Type);
private:
    explicit PhoneTrack();
};

} // namespace
#endif // STUMBLEFISH_PHONETRACK_H

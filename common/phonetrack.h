// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_PHONETRACK_H
#define STUMBLEFISH_PHONETRACK_H

#include <QObject>

namespace Stumblefish {

class PhoneTrack : public QObject
{
    Q_OBJECT
public:
    enum Type {
        NextCloudPhoneTrack,
        SailfishFindMyDevice, // https://sailfishos-chum.github.io/apps/harbour-find-my-device/
        Traccar,
        Custom
    };
    Q_ENUM(Type);
};

} // namespace
#endif // STUMBLEFISH_PHONETRACK_H

// SPDX-License-Identifier: MIT
#ifndef PHONETRACK_SERVICE_H
#define PHONETRACK_SERVICE_H

#include "companionbase.h"
#include <QObject>

class Companion : public StumblefishCompanionBase
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.PhoneTrack")

public:
    explicit Companion(QObject *parent = 0);

private Q_SLOTS:
    void handleDBusMethod() override; // Implement pure virtual method
};
#endif


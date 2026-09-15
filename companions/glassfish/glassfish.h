// SPDX-License-Identifier: MIT
#ifndef GLASSFISH_SERVICE_H
#define GLASSFISH_SERVICE_H

#include "base/companionbase.h"
#include <QObject>

class Companion : public StumblefishCompanionBase
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Lookout")

public:
    explicit Companion(QObject *parent = 0);

private Q_SLOTS:
    void handleDBusMethod() override; // Implement pure virtual method
};
#endif


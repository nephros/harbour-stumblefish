// SPDX-License-Identifier: MIT
#ifndef JOLLAPASS_SERVICE_H
#define JOLLAPASS_SERVICE_H

#include "base/companionbase.h"
#include <QObject>
#include <QDebug>

class Companion : public StumblefishCompanionBase
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.JollaPass")

public:
    explicit Companion(QObject *parent = 0);
//    ~Companion();

private Q_SLOTS:
    void handleDBusMethod() override; // Implement pure virtual method
};
#endif

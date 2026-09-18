// SPDX-License-Identifier: MIT

#include "glassfish.h"
#include <QObject>
#include <QDebug>

//Companion::~Companion(){};

Companion::Companion(QObject *parent)
        : StumblefishCompanionBase(parent)
{
        Q_UNUSED(parent);
};

void Companion::handleDBusMethod()
{
    qDebug() << Q_FUNC_INFO;
    // Implementation
}

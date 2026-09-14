// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_ABSCOMPANION_H
#define STUMBLEFISH_ABSCOMPANION_H

#include <QObject>
#include <QDBusContext>
#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QVariantMap>

#include "base/constants.h"

//class Settings;
class StumblefishCompanionBase : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.BasicCompanion")

protected:
    explicit StumblefishCompanionBase(QObject* parent);
    ~StumblefishCompanionBase();

private Q_SLOTS:
    virtual void handleDBusMethod() = 0; // Pure virtual method
/*
    virtual QVariantMap status() const;
    virtual QVariantMap settings() const;
*/
protected:
    QDBusInterface *m_stumbleService;
    QDBusServiceWatcher m_stumbleWatcher;
};
#endif

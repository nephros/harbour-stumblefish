// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_ABSCOMPANION_H
#define STUMBLEFISH_ABSCOMPANION_H

#include <QObject>
#include <QDBusContext>
#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QVariantMap>

#include "constants.h"

//class Settings;
class StumblefishCompanionBase : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Companion")

protected:
    explicit StumblefishCompanionBase(QObject* parent)
             : QObject(parent) { Q_UNUSED(parent) };
    ~StumblefishCompanionBase() {};

public Q_SLOTS:
    virtual bool ping() { return true; };

protected Q_SLOTS:
    virtual bool enabled() { return m_enabled; };
    virtual QVariantMap status() const = 0; // Pure virtual method
    virtual QVariantMap settings() const = 0; // Pure virtual method

private Q_SLOTS:
    virtual void handleDBusMethod() = 0; // Pure virtual method

protected:
    QDBusInterface *m_stumbleService;
    QDBusServiceWatcher m_stumbleWatcher;

private:
    bool m_enabled;
};
#endif

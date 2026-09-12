// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_ABSCOMPANION_H
#define STUMBLEFISH_ABSCOMPANION_H

#include <QObject>
#include <QDBusContext>
#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QVariantList>
#include <QVariantMap>

#include "common/constants.h"

class Settings;
class StumblefishCompanion : public QObject, protected QDBusContext
{
    Q_OBJECT
    //Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Unknown")

public:
    virtual StumblefishCompanion(QObject *parent = 0);
    virtual StumblefishCompanion();

public Q_SLOTS:

Q_SIGNALS:
private Q_SLOTS:
    virtual QVariantMap status() const;
    virtual QVariantMap settings() const;

private:
    virtual QDBusInterface *m_stumbleService;
    virtual QDBusServiceWatcher m_stumbleWatcher;

    virtual Settings m_settings;
};

#endif

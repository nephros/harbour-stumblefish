// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_ABSCOMPANION_H
#define STUMBLEFISH_ABSCOMPANION_H

#include <QObject>
#include <QDBusContext>
#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QVariantMap>

#include "common/constants.h"

//class Settings;
class StumblefishCompanion : public QObject, protected QDBusContext
{
    Q_OBJECT
    //Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Unknown")

public:
    StumblefishCompanion() = default;
    StumblefishCompanion(QObject* parent) { Q_UNUSED(parent); };
//    explicit StumblefishCompanion(StumblefishCompanion& other);
    virtual ~StumblefishCompanion() = default;

/*
private Q_SLOTS:
    virtual QVariantMap status() const;
    virtual QVariantMap settings() const;

*/
protected:
    QDBusInterface *m_stumbleService;
    QDBusServiceWatcher m_stumbleWatcher;

};

Q_DECLARE_METATYPE(StumblefishCompanion)
#endif

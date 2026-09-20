// SPDX-License-Identifier: MIT
#ifndef GLASSFISH_SERVICE_H
#define GLASSFISH_SERVICE_H

#include "base/companionbase.h"
#include <QObject>

#include<QJsonDocument>

class Companion : public StumblefishCompanionBase
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Lookout")

public:
    explicit Companion(QObject *parent = 0);

public Q_SLOTS:
    QVariantMap settings() const override { return QVariantMap(); };
    QVariantMap status() const override { return QVariantMap(); };

    void analyzeReports();
Q_SIGNALS:
    void alert();
private Q_SLOTS:
    void handleDBusMethod() override; // Implement pure virtual method
private:
    QJsonDocument beaconData;

    bool checkBleEnabled() const;
    QVariantMap callReport() const;
    QList<QVariantMap> getReports(int limit=1) const;
    QVariantMap manufacturerForId(int id);
 };
#endif


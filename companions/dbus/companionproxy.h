// SPDX-License-Identifier: MIT
#ifndef JOLLAPASS_SERVICE_H
#define JOLLAPASS_SERVICE_H

#include <QObject>
#include <QDebug>
#include <QDBusContext>
#include <QDBusInterface>
#include <QException>
#include "base/constants.h"

class CompanionProxy : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.stumblefish.Companions")

public:
    explicit CompanionProxy(QObject *parent = 0);
//    ~Companion();

    void registerCompanion(const QString& name);
    QStringList availableCompanions();
private Q_SLOTS:
    void handleDBusMethod();
    QDBusInterface* ifaceFor(const QString& companion);
private:
    QStringList m_companions;
};

class CouldNotStartException : public QException
{
public:
    void raise() const override { throw *this; }
    CouldNotStartException *clone() const override { return new CouldNotStartException(*this); }
    QString message() { return m_message; };
    int exitCode() { return m_code; };
    void setMessage(const QString& message) { m_message = message; };
    void setCode(int code) { m_code = code; };
private:
    QString m_message;
    int m_code;
};

#endif

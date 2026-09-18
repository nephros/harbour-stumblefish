// SPDX-License-Identifier: MIT
//#include <signal.h>

#include "common/constants.h"
#include <QCoreApplication>
#include <QDebug>
#include "companionproxy.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(QString::fromLatin1(Jollapass::OrganizationName));
    app.setOrganizationDomain(QStringLiteral("stumblefish.org"));
    app.setApplicationName(QString::fromLatin1(Jollapass::ApplicationName));
    app.setApplicationVersion(QStringLiteral(APP_VERSION));

    try {
        CompanionProxy service;
        Q_UNUSED(service);
    } catch (CouldNotStartException &e) {
        qCritical() << "Exiting:" << e.message();
        QCoreApplication::exit(e.exitCode());
    }

    return app.exec();
}

// SPDX-License-Identifier: MIT
#include <signal.h>

#include <QCoreApplication>
#include<QDBusServiceWatcher>
#include<QDBusConnection>
#include<QObject>

#include "common/constants.h"
#include "phonetrackdaemon.h"

#include <QDebug>
namespace {

void signalHandler(int signal)
{
    Q_UNUSED(signal);
    QCoreApplication::quit();
}

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(QString::fromLatin1(Trackfish::OrganizationName));
    app.setOrganizationDomain(QStringLiteral("stumblefish.org"));
    app.setApplicationName(QString::fromLatin1(Trackfish::ApplicationName));
    app.setApplicationVersion(QStringLiteral(APP_VERSION));

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    Companion companion;
    Q_UNUSED(companion);
//    signal(SIGUSR1, service.reloadConfig);

    return app.exec();
}

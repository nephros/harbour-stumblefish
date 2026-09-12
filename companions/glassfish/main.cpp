// SPDX-License-Identifier: MIT
//#include <signal.h>

#include "common/constants.h"
#include <QCoreApplication>
#include <QDebug>
#include "glassfish.h"

namespace {

/*
void signalHandler(int signal)
{
    Q_UNUSED(signal);
    QCoreApplication::quit();
}
*/

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(QString::fromLatin1(Glassfish::OrganizationName));
    app.setOrganizationDomain(QStringLiteral("stumblefish.org"));
    app.setApplicationName(QString::fromLatin1(Glassfish::ApplicationName));
    app.setApplicationVersion(QStringLiteral(APP_VERSION));

    //signal(SIGINT, signalHandler);
    //signal(SIGTERM, signalHandler);

    Companion service;
    Q_UNUSED(service);
//    signal(SIGUSR1, service.reloadConfig);

    return app.exec();
}

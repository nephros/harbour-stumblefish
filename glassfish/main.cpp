#include <QCoreApplication>

#include "../common/constants.h"
#include "glassfish.h"

const char OrganizationName[] = "org.stumblefish";
const char ApplicationName[] = "harbour-glassfish";

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(QString::fromLatin1(OrganizationName));
    app.setOrganizationDomain(QStringLiteral("stumblefish.org"));
    app.setApplicationName(QString::fromLatin1(ApplicationName));
    app.setApplicationVersion(QStringLiteral(APP_VERSION));

    Glassfish* fish = new Glassfish();
    //if (!fish->enabled()) {
    if (0) {
        qDebug() << "Glassfish (or BLE scanning) is not enabled";
        return 1;
    }
    qInfo() << qPrintable(app.applicationName()) << "started";

    QList<QVariantMap> reports = fish->getReports();

    return app.exec();
}

// vim: expandtab ts=4 sw=4 st=4

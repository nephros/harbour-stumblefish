#include <QCoreApplication>

#include "glassfish.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    Glassfish* fish = new Glassfish();
    if (!fish->enabled()) return 1;

    QList<QVariantMap> reports = fish->getReports();

    return app.exec();
}

// vim: expandtab ts=4 sw=4 st=4

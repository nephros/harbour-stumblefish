#include <QCoreApplication>
//#include <QDbusConnection>

#include "glassfish.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    Glassfish* fish = new Glassfish();
    if (!fish->enabled()) return 1;
    return app.exec();
}

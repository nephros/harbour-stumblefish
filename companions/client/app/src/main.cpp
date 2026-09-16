// SPDX-License-Identifier: MIT
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QtQml>
#include <sailfishapp.h>

#include "common/constants.h"
#include "companions/base/constants.h"
#include "src/stumblefishclient.h"
#include "stumblefishcompanionclient.h"
#ifdef TRACK_MY_PHONE
#include "phonetrack/config/phonetrackconfig.h"
#endif

#if !defined(TRACK_MY_PHONE) && !defined(FIND_JOLLA_BUDDIES) && !defined(FIND_KLABAUTERS)
#error None of the companion defines were actually defined!
#endif

int main(int argc, char *argv[])
{
    QGuiApplication *application = SailfishApp::application(argc, argv);
    application->setOrganizationName(QString::fromLatin1(Stumblefish::OrganizationName));
    application->setOrganizationDomain(QStringLiteral("stumblefish.org"));
    application->setApplicationName(QString::fromLatin1(Stumblefish::CompanionAppName));
    application->setApplicationVersion(QStringLiteral(APP_VERSION));

    StumblefishClient client;
    StumblefishCompanionClient companion;

    QQuickView *view = SailfishApp::createView();
    view->rootContext()->setContextProperty(QStringLiteral("stumblefish"), &client);
    view->rootContext()->setContextProperty(QStringLiteral("stumblecompanion"), &companion);
    view->rootContext()->setContextProperty(QStringLiteral("appVersion"), application->applicationVersion());

#ifdef TRACK_MY_PHONE
    Stumblefish::PhoneTrackConfig ptconfig;
    view->rootContext()->setContextProperty(QStringLiteral("PhoneTrackConfig"), &ptconfig);
#endif
    view->engine()->addImportPath(SailfishApp::pathTo(QStringLiteral("lib")).toLocalFile());
    qInfo() << "Found companions:" << companion.availableCompanions().join(",");
    view->setSource((QStringLiteral("/usr/share/harbour-stumblefish/qml/harbour-stumblefish-companion.qml")));
    //view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-stumblefish-companion.qml")));
    view->show();

    return application->exec();
}

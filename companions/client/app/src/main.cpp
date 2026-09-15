// SPDX-License-Identifier: MIT
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QtQml>
#include <sailfishapp.h>

#include "common/constants.h"
#include "src/stumblefishclient.h"
#include "stumblefishcompanionclient.h"

#if !defined(TRACK_MY_PHONE) && !defined(FIND_JOLLA_BUDDIES) && !defined(FIND_KLABAUTERS)
#error None of the companion defines were actually defined!
#endif

int main(int argc, char *argv[])
{
    QGuiApplication *application = SailfishApp::application(argc, argv);
    application->setOrganizationName(QString::fromLatin1(Stumblefish::OrganizationName));
    application->setOrganizationDomain(QStringLiteral("stumblefish.org"));
    application->setApplicationName(QString::fromLatin1(Stumblefish::ApplicationName));
    application->setApplicationVersion(QStringLiteral(APP_VERSION));

//    StumblefishClient client;
    StumblefishCompanionClient companion;

    QQuickView *view = SailfishApp::createView();
//    view->rootContext()->setContextProperty(QStringLiteral("stumblefish"), &client);
    view->rootContext()->setContextProperty(QStringLiteral("stumblefishcompanion"), &companion);
    view->rootContext()->setContextProperty(QStringLiteral("appVersion"), application->applicationVersion());

#ifdef TRACK_MY_PHONE
    companion.registerCompanion(QStringLiteral("PhoneTrack"));
#endif
#ifdef FIND_JOLLA_BUDDIES
    companion.registerCompanion(QStringLiteral("JollaPass"));
#endif
#ifdef FIND_KLABAUTERS
    companion.registerCompanion(QStringLiteral("GlassFish"));
#endif

    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-stumblefish-companion.qml")));
    view->show();

    return application->exec();
}

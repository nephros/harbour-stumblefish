// SPDX-License-Identifier: MIT
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickView>
#include <QQuickItem>
#include <QtQml>
#include <QTranslator>
#include <sailfishapp.h>
#include <libsailfishsilica/silicaitem.h>
#include <libsailfishsilica/silicacontrol.h>

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

const char MainPageStatsIdentifier[] = "Reports";
const char SettingsPageStatsIdentifier[] = "Storage";

const char headersrc[]          = "qrc:/patches/CompanionHeader.qml";
const char glassstatssrc[]      = "qrc:/patches/GlassStats.qml";
const char passstatssrc[]       = "qrc:/patches/PassStats.qml";
const char trackstatssrc[]      = "qrc:/patches/TrackStats.qml";

//const char glasssettingssrc[]   = "qrc:/patches/PhoneTrackSettings.qml";
//const char passsettingssrc[]    = "qrc:/patches/PhoneTrackSettings.qml";
const char tracksettingssrc[]   = "qrc:/patches/PhoneTrackSettings.qml";

static void insertColumnElements(QQuickItem* root, const QList<QString> &sources)
{

    QQmlEngine* engine = QQmlEngine::contextForObject(root)->engine();

    for (const QString& source: sources) {
        QQmlComponent *component = new QQmlComponent(engine, source, root);
        QQuickItem* item = qobject_cast<QQuickItem*>(component->create());
        if (component->isError()) {
            qWarning() << "Failed to create object:" << component->errors();
            return;
        }
        //insert into column
        item->setProperty("width", root->property("width"));
        item->setParentItem(root);  //insert into column
    }

    root->update();
}

static bool patchContents(QQuickView* view)
{
    QQuickItem* found = nullptr;
    for(const auto& child : view->rootObject()->findChildren<QQuickItem*>())
    {
        // look for text string of MainPageStatsIdentifier
        // FIXME: will break when this is i18n-ed.
        if (child->property("text").toString() == QString::fromLatin1(MainPageStatsIdentifier)) {
            found = qobject_cast<QQuickItem*>(child->parent());
            break;
        }
    }

    if(found != nullptr) {
        QList<QString> sources;
        sources << headersrc;
        sources << trackstatssrc;
        sources << glassstatssrc;
        sources << passstatssrc;
        insertColumnElements(found, sources);
    } else {
        qWarning() << "Element to manipulate not found!";
        return false;
    }
    return true;
}

class PSBusyHandler : public QObject
{
    Q_OBJECT
public:
    explicit PSBusyHandler(QObject* parent = 0 ) { ps = parent; };
    ~PSBusyHandler() = default;
public Q_SLOTS:
    void onCurrentPageChanged() const {
        if(!ps) {
            qDebug() << "Pagestack is null!";
            return;
        }
        auto page = ps->property("currentPage").value<QQuickItem*>();
        qDebug() << "Pagestack current page changed" << page;
        if(page->property("defaultTileUrl").isValid()) {
            patchSettingsPage(page);
        }
    }
/*
    void onDepthChanged() const {
        qDebug() << "Pagestack depth changed";
    };
    void onBusyChanged() const {
        qDebug() << "Pagestack busy changed";
    };
*/
private:
    QObject* ps;
    bool patchSettingsPage(QQuickItem* page) const
    {
        qInfo() << "Patching settings page...";
        QQuickItem* found = nullptr;
        for(const auto& child : page->findChildren<QQuickItem*>())
        {
            // FIXME: will break when this is i18n-ed.
            if (child->property("text").toString() == QString::fromLatin1(SettingsPageStatsIdentifier)) {
                found = qobject_cast<QQuickItem*>(child->parent());
                break;
            }
        }
        if(found != nullptr) {
            QList<QString> sources;
            sources << headersrc;
            sources << tracksettingssrc;
            insertColumnElements(found, sources);
        }
        return true;
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QGuiApplication *application = SailfishApp::application(argc, argv);
    application->setOrganizationName(QString::fromLatin1(Stumblefish::OrganizationName));
    application->setOrganizationDomain(QStringLiteral("stumblefish.org"));
    application->setApplicationName(QString::fromLatin1(Stumblefish::CompanionAppName));
    application->setApplicationVersion(QStringLiteral(APP_VERSION));

    QTranslator translator;
    if(translator.load(":/translations/harbour-stumblefish-companion.qm"))
        application->installTranslator(&translator);

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
    view->setSource((QStringLiteral("/usr/share/harbour-stumblefish/qml/harbour-stumblefish.qml")));
//    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-stumblefish.qml")));

    // ping DBus to see what's around
    QStringList companions = companion.availableCompanions();

    qInfo() << "Found companions:" << companions.join(",");
    view->rootContext()->setContextProperty(QStringLiteral("phoneTrackAvailable"),
                                            companions.contains(QString::fromLatin1(Trackfish::ApplicationName)));
    view->rootContext()->setContextProperty(QStringLiteral("glassFishAvailable"),
                                            companions.contains(QString::fromLatin1(Glassfish::ApplicationName)));
    view->rootContext()->setContextProperty(QStringLiteral("passFishAvailable"),
                                            companions.contains(QString::fromLatin1(Jollapass::ApplicationName)));


    // find the pageStack and connect signals:
    if(view->rootObject()->property("pageStack").isValid()) {
        QObject* ps = view->rootObject()->property("pageStack").value<QObject*>();
        PSBusyHandler* handler = new PSBusyHandler(ps);
//        QObject::connect(ps, SIGNAL(busyChanged()), handler, SLOT(onBusyChanged()));
//        QObject::connect(ps, SIGNAL(depthChanged()), handler, SLOT(onDepthChanged()));
        QObject::connect(ps, SIGNAL(currentPageChanged()), handler, SLOT(onCurrentPageChanged()));
    }

    // find the initialPage component, set its objectName property so we find it later:
    QQmlComponent* initialPageComponent = nullptr;
    if(view->rootObject()->property("initialPage").isValid()) {
        qInfo() << "Root has initialPage!";
        initialPageComponent = view->rootObject()->property("initialPage").value<QQmlComponent*>();
        if (initialPageComponent->property("status") == QQmlComponent::Ready) {
            auto *ctx = initialPageComponent->creationContext();
            for (const auto& child : ctx->children()) {
                qInfo() << "initialPage child:" << child;
            }
        }
    }
    // FIXME: Get snippets from companions:
    //companion.modifyContents(view->rootObject());
    qInfo() << "Patching stats page...";
    patchContents(view);

    view->show();

    return application->exec();
}

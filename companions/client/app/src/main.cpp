// SPDX-License-Identifier: MIT
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickView>
#include <QQuickItem>
#include <QtQml>
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

/*
const QByteArray loadersrc = QByteArrayLiteral("import QtQuick 2.0\n\n\
                       Loader {\n\
                            active: true\n\
                            x: Theme.horizontalPageMargin\n\
                            width: parent.width - 2 * Theme.horizontalPageMargin\n\
                            source: Qt.resolvedUrl('plugins/PhoneTrackStats.qml')\n\
                        }\n");
*/
const QByteArray headerqml = QByteArrayLiteral("import QtQuick 2.0\n\
    import Sailfish.Silica 1.0\n\n\
    SectionHeader {\n\
        text: qsTr('Stumblefish Companions')\n\
        font.pixelSize: Theme.fontSizeLarge\n\
    }\n\
    ");
const QByteArray trackstatsqml = QByteArrayLiteral("import QtQuick 2.0\n\
    import Sailfish.Silica 1.0\n\n\
    Column {\n\
        spacing: Theme.paddingSmall\n\
        SectionHeader {\n\
            text: qsTr('Phone Tracking')\n\
        }\n\
        DetailItem {\n\
            label: qsTr('Status')\n\
            }\n\
        DetailItem {\n\
            label: qsTr('Uploaded/Skipped')\n\
        }\n\
    }\n\
    ");
const QByteArray glassstatsqml = QByteArrayLiteral("import QtQuick 2.0\n\
    import Sailfish.Silica 1.0\n\n\
    Column {\n\
        spacing: Theme.paddingSmall\n\
        SectionHeader {\n\
            text: qsTr('Glasses Detection')\n\
        }\n\
        DetailItem {\n\
            label: qsTr('Seen')\n\
            }\n\
        DetailItem {\n\
            label: qsTr('Notified')\n\
        }\n\
    }\n\
    ");

const QByteArray passstatsqml = QByteArrayLiteral("import QtQuick 2.0\n\
    import Sailfish.Silica 1.0\n\n\
    Column {\n\
        spacing: Theme.paddingSmall\n\
        SectionHeader {\n\
            text: qsTr('Jolla Buddies')\n\
        }\n\
        DetailItem {\n\
            label: qsTr('Seen')\n\
            }\n\
        DetailItem {\n\
            label: qsTr('WiFi')\n\
        }\n\
        DetailItem {\n\
            label: qsTr('BT')\n\
        }\n\
        DetailItem {\n\
            label: qsTr('JollaPass Beacon')\n\
        }\n\
    }\n\
    ");


const char MainPageStatsIdentifier[] = "Reports";
const char SettingsPageStatsIdentifier[] = "Storage";

static void insertColumnElements(QQuickItem* root, const QList<QByteArray> &sources)
{

    QQmlEngine* engine = QQmlEngine::contextForObject(root)->engine();

    for (QByteArray source: sources) {
        QQmlComponent *component = new QQmlComponent(engine, root);
        component->setData(source, QUrl());
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
        QList<QByteArray> sources;
        sources << headerqml;
        sources << trackstatsqml;
        sources << glassstatsqml;
        sources << passstatsqml;
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
            QList<QByteArray> sources;
            sources << headerqml;
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
    qInfo() << "Found companions:" << companion.availableCompanions().join(",");

    // find the initialPage component, set its objectName property so we find it later:
    if(view->rootObject()->property("pageStack").isValid()) {
        QObject* ps = view->rootObject()->property("pageStack").value<QObject*>();
        PSBusyHandler* handler = new PSBusyHandler(ps);
//        QObject::connect(ps, SIGNAL(busyChanged()), handler, SLOT(onBusyChanged()));
//        QObject::connect(ps, SIGNAL(depthChanged()), handler, SLOT(onDepthChanged()));
        QObject::connect(ps, SIGNAL(currentPageChanged()), handler, SLOT(onCurrentPageChanged()));
    }
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
/*
        QObject::connect(initialPageComponent,
                         &QQmlComponent::statusChanged,
                         [initialPageComponent](QQmlComponent::Status status) {
                            if (status == QQmlComponent::Ready)
                                qDebug() << "MainPage created.";
                         }
        );
    } else
        qCritical() << "Could not find initialPage!";

*/

    // FIXME: Get snippets from companions:
    //companion.modifyContents(view->rootObject());
    qInfo() << "Patching stats page...";
    patchContents(view);

    view->show();

    return application->exec();
}

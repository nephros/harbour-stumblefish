#include <QObject>
#include <QtQml>

//void qml_register_types_my_module();
//volatile auto registration = &qml_register_types_my_module;
//Q_UNUSED(registration);

class CompanionPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.stumblefish.companions")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("org.stumblefish.companions"));
        qmlRegisterType<CompanionPlugin>(uri, 1, 0, "Companion");
    }
};

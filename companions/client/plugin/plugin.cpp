#include <QtQml>

class TrackFishPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.stumblefish.trackfish")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("org.stumblefish.trackfish"));
        qmlRegisterType<TrackFishPlugin>(uri, 0, 1, "PhoneTrack");
    }
};

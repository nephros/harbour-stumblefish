#include<QObject>
#include<QVariantMap>
#include<QVariantList>

#include<QJsonDocument>

class Glassfish : public QObject
{
    Q_OBJECT
public:
    // TODO: Add a dedicated setting to the daemon, and check for that.
    bool enabled() { return collectingEnabled(); }
    bool collectingEnabled();
public Q_SLOTS:
    void analyzeReports();

Q_SIGNALS:
    void alert();
private:
    QJsonDocument beaconData;

    bool checkBleEnabled() const;
    QVariantMap callReport() const;
    QList<QVariantMap> getReports(int limit=1) const;
};

// vim: expandtab ts=4 sw=4 st=4

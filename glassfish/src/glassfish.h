#include<QObject>
#include<QVariantMap>
#include<QVariantList>

class Glassfish : public QObject
{
  Q_OBJECT
public:
    // TODO: Add a dedicated setting to the daemon, and check for that.
    bool enabled() { return collectingEnabled(); }
    bool collectingEnabled();
    QList<QVariantMap> getReports();
//public Q_SLOTS:

Q_SIGNALS:
    void alert();
private:
    QVariantMap callReport() const;
};

// vim: expandtab ts=4 sw=4 st=4

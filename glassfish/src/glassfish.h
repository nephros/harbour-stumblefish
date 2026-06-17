#include<QObject>

class Glassfish : public QObject
{
  Q_OBJECT
public:
	// TODO: Add a dedicated setting to the daemon, and check for that.
    bool enabled() { return bleCollectionEnabled(); }
    bool bleCollectionEnabled();
//public Q_SLOTS:

Q_SIGNALS:
	void alert();
};

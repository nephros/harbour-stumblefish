#include<QObject>

class Glassfish : public QObject
{
  Q_OBJECT
public:
    bool bleCollectionEnabled();
//public Q_SLOTS:

Q_SIGNALS:
	void alert();
};

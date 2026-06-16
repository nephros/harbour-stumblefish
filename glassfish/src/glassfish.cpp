#include "glassfish.h"
#include "ids.h"

#include "../common/constants.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusError>

bool Glassfish::bleCollectionEnabled()
{
	QDBusMessage reply;
	QDBusMessage message = QDBusMessage::createMethodCall(
								Stumblefish::ServiceName,
								Stumblefish::ObjectPath,
								Stumblefish::InterfaceName,
								QStringLiteral("report")
	);
	reply = QDBusConnection::sessionBus().call(message);
    return false;
}

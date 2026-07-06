#include "uploader.h"
#include "settings.h"

class PhonetrackUploader : public Uploader
{

enum ServiceId {
    Nextcloud,
    Traccar
};

Q_ENUM(ServiceId);


private:
    QUrl formatSubmitUrl(const enum ServiceId service, const QString& id, const QVariantMap& positionData) const;
};
